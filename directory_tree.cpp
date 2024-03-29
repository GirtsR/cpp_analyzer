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
        if (is_directory(x.path()) && x.path().filename().string()[0] != '.') { //Directory found which is not hidden
            std::cout << "Found directory: " << x.path() << std::endl;
            DirectoryTree subdir(x.path());
            subdirectories.push_back(subdir);
            dirsize += subdir.return_dirsize();     //Add subdirectory size to upper dir size
            total_sloc += subdir.return_total_sloc();
            total_cloc += subdir.return_total_cloc();
        } else if (is_regular_file(x.path()) && x.path().filename().string()[0] != '.') {  //File found which is not hidden
            std::cout << "Found file: " << x.path() << std::endl;
            FileStats cur_file(x.path().string());
            dirsize += cur_file.get_size();             //Get file size and add to total
            if (std::find(std::begin(extensions), std::end(extensions), x.path().extension().string()) !=
                std::end(extensions)) {                 //If file is a C++ source code file
                cur_file.check_file();                  //Check how many SLOC and CLOC there are
                total_sloc += cur_file.return_sloc();
                total_cloc += cur_file.return_cloc();
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
        root.add("directory", dirname);     //Only add directory if the folder is not the root dir of the project (project data was added already)
        root.add("size", dirsize);
        root.add("total_sloc", total_sloc);
        root.add("total_cloc", total_cloc);
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

void DirectoryTree::add_history(pt::ptree &root, std::string project, std::string version) {
    fs::path json_dir(project);
    fs::path file;
    bool found_json = false;
    for (fs::directory_entry &x : fs::directory_iterator(json_dir)) {       // Check if project dir contains a [project].json file
        if (is_regular_file(x.path()) && x.path().extension().string() == ".json") {
            file = x.path();
            std::cout << "Found JSON file: " << file.filename() << std::endl;
            found_json = true; // File found
        }
    }
    pt::ptree history_node;
    if (found_json) {
        pt::ptree history;
        pt::read_json(file.string(), history);      //Read data from the JSON file
        for (auto &val : history.get_child("history.")) {                   //Get each element of history array
            pt::ptree old_node;
            old_node.add("version", val.second.get_child("version").data());    //Get data of version field
            old_node.add("size", val.second.get_child("size").data());          //Get data of size field
            old_node.add("sloc", val.second.get_child("sloc").data());          //Get data of sloc field
            old_node.add("cloc", val.second.get_child("cloc").data());          //Get data of cloc field
            history_node.push_back(std::make_pair("", old_node));     //Put in the new property tree
        }
    } else {
        std::cout << "No project history found" << std::endl;
    }
    pt::ptree new_node;
    new_node.put("version", version);          //Put current version
    new_node.put("size", this->dirsize);        //Put current size
    new_node.put("sloc", this->total_sloc);        //Put current sloc count
    new_node.put("cloc", this->total_cloc);        //Put current cloc count
    history_node.push_back(std::make_pair("", new_node));
    root.add_child("history", history_node);
}

unsigned long long DirectoryTree::return_dirsize() {
    return this->dirsize;
}

unsigned long long DirectoryTree::return_total_sloc() {
    return this->total_sloc;
}

unsigned long long DirectoryTree::return_total_cloc() {
    return this->total_cloc;
}