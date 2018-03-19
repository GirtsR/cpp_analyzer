//
// Created by Girts Rudziss on 19/03/2018.
//
#include "file_stats.h"

long long FileStats::get_size(){
    file.seekg(0, std::ios::end);       //Seek to the end of the file
    long long size = file.tellg();      //Converts current position in the file to long long
    file.seekg(0);                      //Seek back to the beginning
    return size;
}