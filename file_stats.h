//
// Created by Girts Rudziss on 19/03/2018.
//
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

namespace fs=boost::filesystem;
#ifndef ANALYZER_FILE_STATS_H
#define ANALYZER_FILE_STATS_H

class FileStats{
    std::string filename;
    fs::path filepath;
    std::shared_ptr<std::ifstream> file;
    unsigned long long size = 0;
    int source_loc = 0;
    int comment_loc = 0;
public:
    /**
     * Construct a FileStats object for storing information about a file
     * @param name - Boost.Filesystem path to file
     */
    FileStats(fs::path name);

    ~FileStats();

    /**
     * Get size of the current file
     * @return size of the file
     */
    long long get_size();

    /**
     * Trim whitespaces from line
     * @param line - current line extracted from the file
     * @return line with no leading and ending whitespaces
     */
    void trim_line(std::string &line);

    /**
     * Check if C++ style comment is a multiple-line comment
     * @param line - current line with trimmed whitespaces
     */
    void multi_line_cpp(std::string &line);

    /**
     * Check if C style comment is a multiple-line comment
     * @param line - current line with trimmed whitespaces
     */
    void multi_line_c(std::string &line);

    /**
     * Check if the comment symbol is within a string literal
     * @param line - current line with trimmed whitespaces
     * @param symbol - symbol that needs to be checked ("//" or "/*")
     * @param quote - quote style
     * @return returns true if the comment symbol is in a string literal
     */
    bool is_in_string(std::string line, std::string symbol, char quote);

    /**
     * Check current line and if it is a source or comment line
     * @param line - current line extracted from the file
     */
    void check_line(std::string &line);

    /**
     * Extract and check all lines from the file
     */
    void check_file();

    /**
     * Print statistics about the current file
     * @param tabs - indentation in std::cout
     */
    void print_stats(std::string tabs);

    std::string return_filename();

    long long return_size();

    int return_sloc();

    int return_cloc();
};

#endif //ANALYZER_FILE_STATS_H
