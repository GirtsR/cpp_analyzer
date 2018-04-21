//
// Created by Girts Rudziss on 22/03/2018.
//
#include "directory_tree.h"

namespace fs=boost::filesystem;
namespace pt=boost::property_tree;

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
                DirectoryTree subdir(x.path());
                subdirectories.push_back(subdir);
                dirsize += subdir.return_dirsize();     //Add subdirectory size to upper dir size
            }
        } else if (is_regular_file(x.path())) {
            std::cout << "File: " << x.path() << std::endl;
            FileStats cur_file(x.path().string());
            dirsize += cur_file.get_size();             //Get file size and add to total
            if (std::find(std::begin(extensions), std::end(extensions), x.path().extension().string()) !=
                std::end(extensions)) {                 //If file is a C++ source code file
                cur_file.check_file();                  //Check how many SLOC and CLOC there are
            }
            files.push_back(cur_file);
        }
    }
}

void DirectoryTree::print_tree(std::string tabs) {
    std::cout << tabs << "Directory " << dirname << " size: " << dirsize << std::endl;
    for (auto file : files) {
        file.print_stats(tabs + "\t");
    }
    for (auto dir : subdirectories) {
        std::cout << tabs << "\tSubdirectory: " << dir.dirname << std::endl;
        dir.print_tree(tabs + "\t");
    }
}

void DirectoryTree::parse_property_tree(pt::ptree &root, bool isfirst) {
    if (!isfirst) {
        root.add("directory", dirname);     //Only add directory if the folder is not the root dir of the project (project name was added already)
        root.add("size", dirsize);
    }
    pt::ptree files_node;
    for (auto file : files) {
        pt::ptree file_node;
        file_node.put("filename", file.return_filename());
        file_node.put("size", file.return_size());
        if (file.return_sloc() > 0 || file.return_cloc() > 0) {
            file_node.put("source_loc", file.return_sloc());
            file_node.put("comment_loc", file.return_cloc());
        }

        files_node.push_back(std::make_pair("", file_node));
    }
    if (!files_node.empty()) {
        root.add_child("files", files_node);
    }

    pt::ptree subdirectories_node;
    for (auto dir : subdirectories) {
        pt::ptree subdirectory_node;
        dir.parse_property_tree(subdirectory_node, false);

        subdirectories_node.push_back(std::make_pair("", subdirectory_node));
    }
    if (!subdirectories_node.empty()) {
        root.add_child("subdirectories", subdirectories_node);
    }
}

void DirectoryTree::add_history(pt::ptree &root, std::string project) {
    fs::path json_dir(project);
    std::vector<fs::path> files;
    for (fs::directory_entry &x : fs::directory_iterator(json_dir)) {
        if (is_regular_file(x.path()) && x.path().extension().string() == ".json") {
            fs::path file(x.path());
            std::cout << "Found JSON file: " << file.filename() << std::endl;
            files.push_back(file);                  //Add all old JSON files to vector
        }
        std::sort(files.begin(), files.end());      //Sort by filename
    }
    pt::ptree history_node;
    std::cout << files.size() << std::endl;
    if (!files.empty()) {
        std::cout << "Latest JSON file: " << files.back() << std::endl;
        fs::path file(files.back().relative_path());    //Get newest JSON file found
        pt::ptree history;
        pt::read_json(file.string(), history);      //Read data from the JSON file
        for (auto& val : history.get_child("history.")) {                   //Get each element of history array
            pt::ptree old_node;
            old_node.add("size", val.second.get_child("size").data());  //Get data of size field
            history_node.push_back(std::make_pair("", old_node));     //Put in the new property tree
        }
    } else {
        std::cout << "No project history found" << std::endl;
    }
    pt::ptree new_node;
    new_node.put("size", this->dirsize);        //Put current size
    history_node.push_back(std::make_pair("", new_node));
    root.add_child("history", history_node);
}


unsigned long long DirectoryTree::return_dirsize() {
    return this->dirsize;
}