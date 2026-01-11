#include "searchengine.h"

#include <iostream>
#include <string>

// Entry point for the CLI application. It prompts the user for a directory
// and search query, performs the search using SearchEngine, and prints
// the results to standard output. This program does not require any
// additional GUI libraries and can be built with a standard C++17
// toolchain.
int main() {
    std::string directory;
    std::cout << "Enter directory path: ";
    std::getline(std::cin, directory);
    if (directory.empty()) {
        std::cerr << "No directory provided. Exiting.\n";
        return 1;
    }
    std::string query;
    std::cout << "Enter word or phrase to search: ";
    std::getline(std::cin, query);
    if (query.empty()) {
        std::cerr << "No search query provided. Exiting.\n";
        return 1;
    }

    auto results = SearchEngine::search(directory, query);
    if (results.empty()) {
        std::cout << "No matches found." << std::endl;
    } else {
        std::cout << "Found " << results.size() << " matches:" << std::endl;
        for (const auto& res : results) {
            std::cout << res.filePath << " (line " << res.lineNumber << "): " << res.lineText << std::endl;
        }
    }
    return 0;
}