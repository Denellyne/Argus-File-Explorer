#include <iostream>
#include <vector>
#include "Functions.h"

int main() {
    int i = 0;
    
    searchPath(i);
    std::vector<Directory> directories(i, Directory());
    createPathVector(directories);
    std::cout << directories[3].filePath << '\n';
    //File.getSize();

    return 0;
}