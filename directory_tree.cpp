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
            std::cout << "Folder " << x.path() << std::endl;
            subdirectories.push_back(DirectoryTree(x.path()));
        } else if (is_regular_file(x.path())) {
            std::cout << "File: " << x.path() << std::endl;
            if (x.path().extension().string() == ".cpp" || x.path().extension().string() == ".h") {
                FileStats cur_file(x.path().string());
                cur_file.check_file();
                files.push_back(cur_file);
            }
        }
    }
}

void DirectoryTree::print_tree(std::string tabs) {
    std::cout << tabs << "Directory " << dirname << " contains: " << std::endl;
    for (auto file : files) {
        file.print_stats(tabs+"\t");
    }
    for (auto dir : subdirectories) {
        std::cout << tabs << "\tSubdirectory: " << dir.dirname << std::endl;
        dir.print_tree(tabs+"\t");
    }
}
