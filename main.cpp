#include <iostream>
#include <vector>
#include "Functions.h"

int main() {

    std::vector<Directory> directories(0,Directory());
    std::vector<File> files(0,File());
    setDirectoriesFiles(directories,files);
    for (int i = 0;i < directories.size(); i++) {
        std::cout << directories[i].filePath << '\n';
    }
    for (int i = 0;i < files.size(); i++) {
        std::cout << files[i].filePath << std::endl;
    }

    return 0;
}