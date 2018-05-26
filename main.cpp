//
// Created by Girts Rudziss on 14/03/2018.
//
#include "directory_tree.h"
#include "html_generator.h"

namespace fs=boost::filesystem;
namespace pt=boost::property_tree;

int main(int argc, char *argv[]) {

    if (argc < 4) {
        if (argc == 1) {
            std::cout << "No project name was specified" << std::endl;
        } else if (argc == 2) {
            std::cout << "No root directory for the project was specified" << std::endl;
        }
        else if (argc == 3) {
            std::cout << "No project version specified" << std::endl;
        }
        std::cout << "Usage: ./analyzer [Project name] [Path to project root] [Project version]" << std::endl;
        return 1;
    }

    std::string project = argv[1];
    fs::path p(argv[2]);
    std::string version = argv[3];

    if (exists(p) && is_directory(p)) {
        std::cout << "Tool for static analysis of C++ files has started successfully for the project:" + project << std::endl;

        DirectoryTree tree(p);        // Start iterating through the project directories

        std::string filename = project + ".json";

        fs::path folder = project;
        if (!fs::exists(folder)) {
            if (fs::create_directory(folder)) {
                std::cout << "Folder " << folder << " created successfully!" << std::endl;
            } else {
                std::cout << "Folder could not be created! Please check the project name specified is valid"
                    " or if administrator privileges are needed" << std::endl;
                return 1;
            }
        } else std::cout << "Folder " << folder << " exists already, no need to create" << std::endl;

        pt::ptree root;                 //Construct Boost.PropertyTree object
        root.put("project", project);
        root.put("version", version);
        root.put("totalsize", tree.return_dirsize());
        root.add("totalsloc", tree.return_total_sloc());
        root.add("totalcloc", tree.return_total_cloc());
        root.put("unit", "bytes");
        std::cout << "Starting JSON parse" << std::endl;
        tree.parse_property_tree(root, true);
        tree.add_history(root, project, version);
        pt::write_json("./" + project + "/" + filename, root);
        std::cout << "JSON parse finished! Output file: " << filename << std::endl;

        generate_report(root, project);
    } else {
        std::cout << "Path to the root directory is invalid" << std::endl;
        return 1;
    }
    return 0;
}