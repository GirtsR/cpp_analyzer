//
// Created by Girts Rudziss on 19/03/2018.
//
#include <iostream>
#include <fstream>

#ifndef ANALYZER_FILE_STATS_H
#define ANALYZER_FILE_STATS_H

class FileStats{
    std::string filename;
    std::ifstream file;
    long long size;
    int source_loc;
    int comment_loc;
public:
    FileStats(){};

    ~FileStats(){};

    long long get_size();
};

#endif //ANALYZER_FILE_STATS_H
