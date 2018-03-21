//
// Created by Girts Rudziss on 19/03/2018.
//
#include "file_stats.h"

FileStats::FileStats(std::string name) {
    file.open(name, std::ifstream::in);
    filename = name;
}

FileStats::~FileStats() {
    if (file.is_open()) {
        file.close();
    }
}

void FileStats::get_size() {
    file.seekg(0, std::ios::end);       //Seek to the end of the file
    size = file.tellg();                //Converts size to offset of file end from the start of the file
    file.seekg(0);                      //Seek back to the beginning
}

std::string FileStats::trim_line(std::string &line) {
    if (line.empty()) return line;      //If the line is empty - do nothing
    while (line.at(0) == ' ') {
        line.erase(line.begin());       //Erase leading whitespaces
    }
    while (line.at(line.size() - 1) == ' ') {
        line.erase(line.end());         //Erase ending whitespaces
    }
    return line;
}

void FileStats::multi_line_cpp(std::string &line) {
    //TODO - handle cases where the comment symbols are in a string literal
    comment_loc++;                               //First line is definitely a comment
    while (line.at(line.size() - 1) == '\\') {     // The comment can be a multi-line comment, if a backslash ("\") is found at the end
        comment_loc++;
        if (!file.eof()) {
            getline(file, line); //Get line if end of file has not yet been reached
            line = trim_line(line);
        } else return;
    }
}

void FileStats::multi_line_c(std::string &line) {
    //TODO - handle cases where the comment symbols are in a string literal (or a comment)
    comment_loc++;
    bool is_one_line = true;                        //Comment can be a one line comment as well - special case
    while (line.find("*/") == std::string::npos) {     // Loops until the characters "*/" are found in a line
        comment_loc++;
        is_one_line = false;
        if (!file.eof()) {
            getline(file, line);
            trim_line(line);
        } else return;

    }
    if (line.find("*/") != line.size() - 2) {          // If the line does not end with "*/" - more code or comments are present in the line
        if (is_one_line) {                               //Special case to check if a c-style comment is before or between code
            std::string part1 = line.substr(0, line.find("/*"));         //Get the first part of the line - before the start of the comment
            if (!part1.empty()) {                                    //If the first part is not empty - code appears there
                std::string part2 = line.substr(line.find("*/") + 2);
                trim_line(part2);
                if (part2.find("//") == 0 || part2.find("/*") == 0) {
                    comment_loc--;                  //2 comments or more found in the same line - should count as one comment
                } else if (part2.find("//") != std::string::npos || part2.find("/*") != std::string::npos) {
                    source_loc--;           //2 code blocks, at least 2 comments found
                    comment_loc--;

                } else source_loc--;                  //2 code blocks (e.g. "code /* comment */ code") found - should count as one source line of code
            }

        } else {
            std::string part2 = line.substr(line.find("*/") + 2);
            if (part2.find("//") != std::string::npos || part2.find("/*") != std::string::npos) {
                comment_loc--;                      //2 comments or more found in the same line - should count as one comment
            }
        }
        line = line.substr(line.find("*/") + 2);       // Get the part of the line that follows the comment
        check_line(line);
    }
}

void FileStats::check_line(std::string &line) {
    trim_line(line);
    //std::cout << line << std::endl;
    if (line.find("//") == std::string::npos &&
        line.find("/*") == std::string::npos) {      //Line does not contain any comment (is a source code line)
        source_loc++;
    } else if (line.find("//") == 0) {            //Whole line is a C++ style comment
        multi_line_cpp(line);

    } else if (line.find("/*") == 0) {              //Line is a C style comment
        multi_line_c(line);
    } else if (line.find("//") != std::string::npos) {         //C++ comment found after code
        source_loc++;
        multi_line_cpp(line);
    } else if (line.find("/*") != std::string::npos) {         //C comment found before or after code
        source_loc++;
        multi_line_c(line);
    }
}

void FileStats::check_file() {
    get_size();
    std::string line;
    while (!file.eof()) {
        getline(file, line);
        if (line.empty()) continue;         //Line is empty - not a code or comment line - can be skipped
        else {
            check_line(line);
        }
    }
}

void FileStats::print_stats() {
    std::cout << "File " << filename << " statistics:" << std::endl;
    std::cout << "\tFile size is " << size << " bytes." << std::endl;
    std::cout << "\tFile contains " << source_loc << " source lines of code." << std::endl;
    std::cout << "\tFile contains " << comment_loc << " comment lines." << std::endl;
}



