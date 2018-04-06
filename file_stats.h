//
// Created by Girts Rudziss on 19/03/2018.
//
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

namespace fs=boost::filesystem;
#ifndef ANALYZER_FILE_STATS_H
#define ANALYZER_FILE_STATS_H

class FileStats {
    std::string filename;
    fs::path filepath;
    std::shared_ptr<std::ifstream> file;
    long long size = 0;
    int source_loc = 0;
    int comment_loc = 0;
public:
    FileStats(fs::path name);

    ~FileStats();

    void get_size();

    void trim_line(std::string &line);

    void multi_line_cpp(std::string &line);

    void multi_line_c(std::string &line);

    bool is_in_string(std::string line, std::string symbol, char quote);

    void check_line(std::string &line);

    void check_file();

    void print_stats(std::string tabs);

};

#endif //ANALYZER_FILE_STATS_H
