//
// Created by Girts Rudziss on 19/03/2018.
//
#include <iostream>
#include <fstream>

#ifndef ANALYZER_FILE_STATS_H
#define ANALYZER_FILE_STATS_H

class FileStats{
    std::string filename;
    std::shared_ptr<std::ifstream> file;
    long long size = 0;
    int source_loc = 0;
    int comment_loc = 0;
public:
    FileStats(std::string name);

    ~FileStats();

    void get_size();

    std::string trim_line(std::string& line);

    void multi_line_cpp(std::string& line);

    void multi_line_c(std::string& line);

    bool is_in_string(std::string line, std::string symbol, char quote);

    void check_line(std::string& line);

    void check_file();

    void print_stats(std::string tabs);

};

#endif //ANALYZER_FILE_STATS_H
