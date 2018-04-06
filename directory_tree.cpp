//
// Created by Girts Rudziss on 22/03/2018.
//
#include "directory_tree.h"

namespace fs=boost::filesystem;

DirectoryTree::DirectoryTree(fs::path path) {
    dirpath = path;
    dirname = path.filename().string();
    iterate();
}

DirectoryTree::~DirectoryTree() {

}

void DirectoryTree::iterate() {
    for (fs::directory_entry &x : fs::directory_iterator(dirpath)) {
        if (is_directory(x.path())) {
            if (x.path().filename().string()[0] != '.') {           //Check if folder is not hidden
                std::cout << "Folder " << x.path() << std::endl;
                subdirectories.push_back(DirectoryTree(x.path()));
            }
        } else if (is_regular_file(x.path())) {
            std::cout << "File: " << x.path() << std::endl;
            FileStats cur_file(x.path().string());
            cur_file.get_size();
            if (std::find(std::begin(extensions), std::end(extensions), x.path().extension().string()) !=
                std::end(extensions)) {                 //If file is a C++ source code file
                cur_file.check_file();                  //Check how many SLOC and CLOC there are
            }
            files.push_back(cur_file);
        }
    }
}

void DirectoryTree::print_tree(std::string tabs) {
    std::cout << tabs << "Directory " << dirname << " contains: " << std::endl;
    for (auto file : files) {
        file.print_stats(tabs + "\t");
    }
    for (auto dir : subdirectories) {
        std::cout << tabs << "\tSubdirectory: " << dir.dirname << std::endl;
        dir.print_tree(tabs + "\t");
    }
}
