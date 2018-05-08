#include <iostream>

//ASD\
as\
aģ


int main() {
    /* Multi line
    end */ int count = 0; //asa
    std::cout << "Hello, World!" << std::endl; /* one line */ std::cout << "//LOL";
    std::cout << "//Not a line comment" << "And \" this /* is not a comment as well";
    return 0;
}
//7 SLOC and 7 CLOC expected ( regex ^.+$ to check full line count)