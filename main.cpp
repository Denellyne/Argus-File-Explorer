#include <iostream>
#include <vector>
#include <stack>
#include "Functions.h"

int main() {
    std::stack<std::string> dirStack;
    std::vector<Directory> directories(0, Directory("C:\\"));
    std::vector<File> files(0,File("fileExplorer.exe"));
    searchPath("C:\\", directories, files, dirStack);
    bool a = true;
    while (a) {   
        std::string newPath;
        std::getline(std::cin, newPath);
        searchPath(newPath, directories, files, dirStack);
        debugPrint(directories, files);
    }
    return 0;
}