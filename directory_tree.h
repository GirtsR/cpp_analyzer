//
// Created by Girts Rudziss on 22/03/2018.
//
#include "file_stats.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace fs=boost::filesystem;
namespace pt=boost::property_tree;
#ifndef ANALYZER_DIRECTORY_TREE_H
#define ANALYZER_DIRECTORY_TREE_H

class DirectoryTree {
    fs::path dirpath;
    std::string dirname;
    std::vector<FileStats> files;
    std::vector<DirectoryTree> subdirectories;
    std::string extensions[7] = {".cpp", ".cc", ".cxx", ".cc",
                                 ".h", ".hh", ".hpp"};
    unsigned long long dirsize = 0;
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

    /**
     * Parse DirectoryTree object to Boost Property tree
     * @param root - reference to Property tree root
     * @param isfirst - true if passed first time (root directory, directory name will not be added to Property tree)
     */
    void parse_property_tree(pt::ptree &root, bool isfirst);

    unsigned long long return_dirsize();
};

#endif //ANALYZER_DIRECTORY_TREE_H
