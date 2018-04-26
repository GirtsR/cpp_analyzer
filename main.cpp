//
// Created by Girts Rudziss on 14/03/2018.
//
#include "directory_tree.h"
#include <boost/date_time.hpp>

namespace fs=boost::filesystem;
namespace pt=boost::property_tree;
namespace dt=boost::posix_time;

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cout << "Usage: ./analyzer [Project name] [Path to project root]" << std::endl;
        return 0;
    }

    std::string project = argv[1];
    fs::path p(argv[2]);

    if (exists(p)) {
        if (is_regular_file(p)) {
            std::cout << "Path to project is a file: " << p << ", not a directory" << std::endl;
            return 0;
        } else if (is_directory(p)) {
            std::cout << p << " is a directory containing:\n";
            DirectoryTree tree(p);
            //tree.print_tree("");

            dt::ptime curTime = dt::second_clock::local_time();
            std::string time = dt::to_iso_string(curTime);
            std::string filename = project + "-" + time + ".json";

            //TODO - check for exceptions and errors
            fs::path folder = project;
            if (!fs::exists(folder)) {
                if (fs::create_directory(folder)) {
                    std::cout << "Folder " << folder << " created successfully!" << std::endl;
                } else {
                    std::cout << "Folder could not be created! Please check the project name specified is valid"
                            " or if administrator privileges are needed" << std::endl;
                    return 0;
                }
            } else std::cout << "Folder " << folder << " exists already, no need to create" << std::endl;

            pt::ptree root;
            root.put("project", project);
            root.put("totalsize", tree.return_dirsize());
            root.put("unit", "bytes");
            std::cout << "Starting JSON parse" << std::endl;
            tree.parse_property_tree(root, true);
            tree.add_history(root, project);
            pt::write_json("./" + project + "/" + filename, root);
            std::cout << "JSON parse finished! Output file: " << filename << std::endl;
            // std::cout << "XML output" << std::endl;
            // pt::write_xml(std::cout, root, pt::xml_writer_make_settings<pt::ptree::key_type>(' ', 4));

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