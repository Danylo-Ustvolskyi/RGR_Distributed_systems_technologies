#pragma once

#include <string>
#include <vector>

// SearchResult represents a single occurrence of the search query in a file.
// filePath   – absolute or relative path to the file where the match was found
// lineNumber – 1‑based index of the line containing the match
// lineText   – contents of that line (without newline). It is stored as a
//             std::string so that it can be converted to QString easily in
//             the GUI. For very long lines consider truncating when
//             displaying to the user.
struct SearchResult {
    std::string filePath;
    std::size_t lineNumber;
    std::string lineText;
};

// SearchEngine encapsulates the functionality required to perform a
// multi‑threaded search for a string within a directory tree. It is
// implemented as a simple utility class with a single static function
// returning a vector of SearchResult objects. The search operation is
// blocking; it creates a number of worker threads equal to the
// hardware concurrency (or fewer if there are fewer files) and divides
// the workload evenly among them. Each thread reads its assigned files
// line‑by‑line and performs a straightforward substring search.
//
// Usage example:
//   auto results = SearchEngine::search("/path/to/root", "error");
//   for (const auto& res : results) {
//       std::cout << res.filePath << ":" << res.lineNumber << ": " << res.lineText << '\n';
//   }
class SearchEngine {
public:
    // Performs a case‑sensitive search for `query` starting from the
    // directory `rootDir`. Only files with the given `extension` are
    // processed (default ".txt"). If extension is an empty string,
    // all files are searched. The function returns a vector of
    // SearchResult objects containing the file path, line number, and
    // line contents for each match. If the directory does not exist
    // or no files match the extension, the returned vector will be
    // empty.
    static std::vector<SearchResult> search(const std::string& rootDir,
                                            const std::string& query,
                                            const std::string& extension = ".txt");
};