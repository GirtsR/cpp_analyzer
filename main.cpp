//
// Created by Girts Rudziss on 14/03/2018.
//
#include "directory_tree.h"
namespace fs=boost::filesystem;

int main(int argc, char *argv[]) {

    if (argc < 2)
    {
        std::cout << "Usage: ./analyzer [path]" << std::endl;
        return 1;
    }

    fs::path p (argv[1]);

    try
    {
        if (exists(p))
        {
            if (is_regular_file(p))
                std::cout << p << " size is " << file_size(p) << '\n';

            else if (is_directory(p))
            {
                std::cout << p << " is a directory containing:\n";
               // iterate(p);
                DirectoryTree tree(p);
                tree.print_tree("");
            }
            else
                std::cout << p << " exists, but is not a regular file or directory\n";
        }
        else
            std::cout << p << " does not exist\n";
    }

    catch (const fs::filesystem_error& ex)
    {
        std::cout << ex.what() << '\n';
    }
    return 0;
}