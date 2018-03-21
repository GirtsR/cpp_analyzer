//
// Created by Girts Rudziss on 14/03/2018.
//
#include <iostream>
#include <fstream>
#include "file_stats.h"

int main(int argc, char *argv[]) {
    FileStats cur_file("test.cpp");
    std::string line;
    cur_file.check_file();
    cur_file.print_stats();
    return 0;
}
