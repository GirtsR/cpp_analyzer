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
    DirectoryTree(fs::path path);

    ~DirectoryTree();

    void iterate();

    void print_tree(std::string tabs);

};

#endif //ANALYZER_DIRECTORY_TREE_H
