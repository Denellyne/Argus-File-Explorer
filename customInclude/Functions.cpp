#include "Functions.h"
#include "Drive.h"
#include "File.h"
#include "Directory.h"
#include <iostream>
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;


void setDirectoriesFiles(std::string Path,std::vector<Directory>& directories, std::vector<File>& files) {
    files.clear();
    directories.clear();
    for (const auto& dirEntry : fs::directory_iterator(Path, fs::directory_options::skip_permission_denied)){
        if (dirEntry.is_directory()) {
            directories.push_back(Directory(dirEntry.path().generic_string()));
        }
        else{
            files.push_back(File(dirEntry.path().generic_string()));
        }
    }
}

void function1Test(std::vector<Directory>& directories, std::vector<File>& files) {
    std::cout << "Directories:\n";
    for (int i = 0;i < directories.size(); i++) {
        std::cout << directories[i].filePath << '\n';
    }
    std::cout << "Files:\n";
    for (int i = 0;i < files.size(); i++) {
        std::cout << files[i].filePath << std::endl;
    }
}