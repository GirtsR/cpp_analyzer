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
    unsigned long long total_sloc = 0;
    unsigned long long total_cloc = 0;
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
     * @param tabs - indentation of std::cout
     */
    void print_tree(std::string tabs);

    /**
     * Parse DirectoryTree object to Boost Property tree
     * @param root - reference to Property tree root
     * @param isfirst - true if passed first time (root directory, directory name will not be added to Property tree)
     */
    void parse_property_tree(pt::ptree &root, bool isfirst);

    /**
     * Add size of previous project versions from the previous .json file
     * @param root - reference to Property tree root
     * @param project - name of project passed by the user
     * @param version - project version passed by the user
     */
    void add_history(pt::ptree &root, std::string project, std::string version);

    unsigned long long return_dirsize();

    unsigned long long return_total_sloc();

    unsigned long long return_total_cloc();
};

#endif //ANALYZER_DIRECTORY_TREE_H
