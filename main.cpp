#include <iostream>
#include <fstream>

long long get_size(std::ifstream &file) {
    file.seekg(0, std::ios::end);
    long long size = file.tellg();
    file.seekg(0);
    return size;
}

int main(int argc, char *argv[]) {
    std::ifstream file("test.cpp");
    std::string line;
    if (file.is_open()) {
        std::cout << "File size is " << get_size(file) << " bytes" << std::endl;
    } else {
        std::cout << "Error opening file" << std::endl;
        return 0;
    }
}
