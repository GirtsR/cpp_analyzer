//
// Created by Girts Rudziss on 19/03/2018.
//
#include "file_stats.h"
namespace fs=boost::filesystem;

FileStats::FileStats(fs::path path) {
    filepath = path;
    filename = path.filename().string();
    file = std::make_shared<std::ifstream>(filepath.string(), std::ifstream::in);
    if (!file->is_open()) {
        std::string message = "Could not open file " + filename;
        throw std::runtime_error(message);
    }
}

FileStats::~FileStats() {
    if (file->is_open()) {
        file->close();
    }
}

long long FileStats::get_size() {
    file->seekg(0, std::ios::end);       //Seek to the end of the file
    size = file->tellg();                //Converts size to offset of file end from the start of the file
    file->seekg(0);                      //Seek back to the beginning
    return size;
}

void FileStats::trim_line(std::string &line) {
    if (line.empty()) return;      //If the line is empty - do nothing
    while (line[0] == ' ') {
        line.erase(0, 1);       //Erase leading whitespaces
    }
    while (line[line.size() - 1] == ' ') {
        line.pop_back();         //Erase ending whitespaces
    }
}

void FileStats::multi_line_cpp(std::string &line) {
    comment_loc++;                               //First line is definitely a comment
    while (line.at(line.size() - 1) == '\\') {     // The comment can be a multi-line comment, if a backslash ("\") is found at the end of the line
        comment_loc++;
        if (!file->eof()) {
            getline(*file, line); //Get line if end of file has not yet been reached
            trim_line(line);
        } else return;
    }
}

void FileStats::multi_line_c(std::string &line) {
    comment_loc++;
    bool is_one_line = true;                        //Comment can be a one line comment as well - special case
    while (line.find("*/") == std::string::npos) {     // Loops until the characters "*/" are found in a line
        comment_loc++;
        is_one_line = false;
        if (!file->eof()) {
            getline(*file, line);
            trim_line(line);
        } else return;

    }
    if (line.find("*/") != line.size() - 2) {          // If the line does not end with "*/" - more code or comments are present in the line
        if (is_one_line) {                               //Special case to check if a c-style comment is before or between code
            std::string part1 = line.substr(0, line.find("/*"));         //Get the first part of the line - before the start of the comment
            if (!part1.empty()) {                                    //If the first part is not empty - code appears there
                std::string part2 = line.substr(line.find("*/") + 2);   //Get the second part - after the comment
                trim_line(part2);
                if (part2.find("//") == 0 || part2.find("/*") == 0) {
                    comment_loc--;                  //2 comments or more found in the same line - should count as one comment
                } else if (part2.find("//") != std::string::npos || part2.find("/*") != std::string::npos) {
                    if (!is_in_string(part2, "/*", '\"') && !is_in_string(part2, "//", '\"')) {     //String literal check
                        comment_loc--;
                        source_loc--;            //2 code blocks and 2 or more comment lines found
                    } else {
                        source_loc--;           //Only 2 or more code blocks found - should count as one
                    }

                } else source_loc--;                  //2 code blocks (e.g. "code /* comment */ code") found - should count as one source line of code
            }

        } else {
            std::string part2 = line.substr(line.find("*/") + 2);
            if (part2.find("//") != std::string::npos || part2.find("/*") != std::string::npos) {
                if (!is_in_string(part2, "/*", '\"') && !is_in_string(part2, "//", '\"'))
                    comment_loc--;                      //2 comments or more found in the same line - should count as one comment
            }
        }
        line = line.substr(line.find("*/") + 2);       // Get the part of the line that follows the comment
        check_line(line);
    }
}

bool FileStats::is_in_string(std::string line, std::string symbol, char quote) {          //Check if comment symbol is in a string literal (between " ")
    int sympos = line.find(symbol);
    if (sympos == std::string::npos) return false;
    if (line.find_last_of(quote, sympos) < sympos) {
        int pos1 = line.find_last_of(quote, sympos);
        while (line[pos1 - 1] == '\\') {                             //The previous character before the pos1 quote is \ - double quote is an escaped character
            if (line.find_last_of(quote, pos1 - 2) != std::string::npos) {
                pos1 = line.find_last_of(quote, pos1 - 2);
            } else return false;
        }
        int pos2 = line.find(quote, sympos);
        if (pos2 > sympos && pos2 != std::string::npos) {           //Symbol is between 2 double quotes
            while (line[pos2 - 1] == '\\') {                    //The previous character before the pos2 quote is \ - double quote is an escaped character
                size_t test = line.find(quote, pos2 + 2);
                if (line.find(quote, pos2 + 2) != std::string::npos) {
                    pos2 = line.find(quote, pos2 + 2);
                } else return false;
            }
            int count = 0;
            bool between = true;
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == quote) {
                    if (line[i - 1] != '\\') count++;
                    if (pos1 == i) {                   //Symbol is in a string literal if pos1 count % 2 == 1 and pos2 count % 2 == 0
                        if (count % 2 != 1) {
                            between = false;
                            break;
                        }
                    }
                    if (pos2 == i) {
                        if (count % 2 != 0) {
                            between = false;
                            break;
                        }
                    }
                }
            }
            return between;
        }
    }
    return false;
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
    } else if (line.find("//") != std::string::npos &&
               !(is_in_string(line, "//", '\"'))) {      //C++ comment found after code
        source_loc++;
        multi_line_cpp(line);
    } else if (line.find("/*") != std::string::npos &&
               !(is_in_string(line, "/*", '\"'))) {       //C comment found after or between code
        source_loc++;
        multi_line_c(line);
    } else {                    //Line needs to be subtracted to start after the first string literal and then checked again
        size_t cpp_pos = line.find("//");
        size_t c_pos = line.find("/*");
        if (is_in_string(line, "//", '\"') && is_in_string(line, "/*", '\"')) {   //Both symbols are in string literals
            if (c_pos < cpp_pos) {
                line = line.substr(line.find('\"', c_pos + 1) + 1);
            } else line = line.substr(line.find('\"', cpp_pos + 1) + 1);
        } else if (is_in_string(line, "//", '\"')) {                //Only "//" is in a string literal
            line = line.substr(line.find('\"', cpp_pos + 1) + 1);
        } else if (is_in_string(line, "/*", '\"')) {                //Only "/*" is in a string literal
            line = line.substr(line.find('\"', c_pos + 1) + 1);
        } else return;
        check_line(line);
    }
}

void FileStats::check_file() {
    std::string line;
    while (!file->eof()) {
        getline(*file, line);
        if (line.empty()) continue;         //Line is empty - not a code or comment line - can be skipped
        else {
            check_line(line);
        }
    }
}

void FileStats::print_stats(std::string tabs) {
    std::cout << tabs << "File " << filename << " statistics:" << std::endl;
    std::cout << tabs << "\tFile size is " << size << " bytes." << std::endl;
    if (source_loc > 0 || comment_loc > 0) {
        std::cout << tabs << "\tFile contains " << source_loc << " source lines of code." << std::endl;
        std::cout << tabs << "\tFile contains " << comment_loc << " comment lines." << std::endl;
    }
}

std::string FileStats::return_filename() {
    return this->filename;
}

long long FileStats::return_size() {
    return this->size;
}

int FileStats::return_sloc() {
    return this->source_loc;
}

int FileStats::return_cloc() {
    return this->comment_loc;
}
