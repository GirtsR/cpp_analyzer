//
// Created by Girts Rudziss on 14/03/2018.
//
#include "directory_tree.h"

namespace fs=boost::filesystem;
namespace pt=boost::property_tree;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./analyzer [Path to project root]" << std::endl;
        return 0;
    }

    fs::path p(argv[1]);

    if (exists(p)) {
        if (is_regular_file(p)) {
            std::cout << "Path to project is a file: " << p << ", not a directory" << std::endl;
            return 0;
        } else if (is_directory(p)) {
            std::cout << p << " is a directory containing:\n";
            DirectoryTree tree(p);
            tree.print_tree("");
            pt::ptree root;
            std::cout << "Starting JSON parse" << std::endl;
            tree.parse_property_tree(root);
            pt::write_json(std::cout, root);
            std::cout << "Finished!" << std::endl;
            std::cout << "XML output" << std::endl;
            pt::write_xml(std::cout, root, pt::xml_writer_make_settings<pt::ptree::key_type>(' ', 4));
        } else {
            std::cout << p << " exists, but is not a regular file or directory" << std::endl;
            return 0;
        }

    } else {
        std::cout << p << " does not exist" << std::endl;
        return 0;
    }
    return 1;
}