#include "searchengine.h"

#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

// Helper function to perform the per‑file search. It reads lines from
// the given file and appends matches to the supplied localResults vector.
static void searchInFile(const std::string& file,
                         const std::string& query,
                         std::vector<SearchResult>& localResults) {
    std::ifstream in(file);
    if (!in.is_open()) {
        return;
    }
    std::string line;
    std::size_t lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        // perform a simple substring search; this is case sensitive
        if (!query.empty() && line.find(query) != std::string::npos) {
            SearchResult res;
            res.filePath = file;
            res.lineNumber = lineNo;
            res.lineText = line;
            localResults.emplace_back(std::move(res));
        }
    }
}

std::vector<SearchResult> SearchEngine::search(const std::string& rootDir,
                                               const std::string& query,
                                               const std::string& extension) {
    std::vector<std::string> files;
    files.reserve(64);
    // Traverse the directory tree recursively. If rootDir does not exist
    // std::filesystem will throw; we catch and return empty results.
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDir)) {
            if (entry.is_regular_file()) {
                if (extension.empty() || entry.path().extension() == extension) {
                    files.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error&) {
        // In case of an invalid root directory or insufficient permissions
        return {};
    }

    std::vector<SearchResult> results;
    std::mutex resultsMutex;

    std::size_t fileCount = files.size();
    if (fileCount == 0) {
        return results;
    }

    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) {
        threadCount = 2; // fall back to a reasonable default
    }
    // Do not spawn more threads than there are files
    if (fileCount < threadCount) {
        threadCount = static_cast<unsigned int>(fileCount);
    }

    std::vector<std::thread> workers;
    workers.reserve(threadCount);

    for (unsigned int t = 0; t < threadCount; ++t) {
        workers.emplace_back([&, t]() {
            std::vector<SearchResult> localResults;
            // Round‑robin assignment of files to threads. Each thread starts at
            // its index and processes every threadCount‑th file.
            for (std::size_t i = t; i < fileCount; i += threadCount) {
                const std::string& f = files[i];
                searchInFile(f, query, localResults);
            }
            if (!localResults.empty()) {
                std::lock_guard<std::mutex> lock(resultsMutex);
                results.insert(results.end(),
                               std::make_move_iterator(localResults.begin()),
                               std::make_move_iterator(localResults.end()));
            }
        });
    }
    for (auto& thr : workers) {
        thr.join();
    }
    return results;
}