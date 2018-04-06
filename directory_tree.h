//
// Created by Girts Rudziss on 22/03/2018.
//
#include "file_stats.h"

namespace fs=boost::filesystem;
#ifndef ANALYZER_DIRECTORY_TREE_H
#define ANALYZER_DIRECTORY_TREE_H

class DirectoryTree {
    fs::path dirpath;
    std::string dirname;
    std::vector<FileStats> files;
    std::vector<DirectoryTree> subdirectories;
    std::string extensions[7] = {".cpp", ".cc", ".cxx", ".cc",
                                 ".h", ".hh", ".hpp"};
public:
    /**
     * Construct a DirectoryTree object for storing its files and subdirectories
     * @param path - Boost.Filesystem path to directory
     */
    DirectoryTree(fs::path path);

    ~DirectoryTree();

    /**
     * Iterate through the directories
     */
    void iterate();

    /**
     * Print DirectoryTree, its files un subdirectories
     * @param tabs - indentation if std::cout
     */
    void print_tree(std::string tabs);

};

#endif //ANALYZER_DIRECTORY_TREE_H
