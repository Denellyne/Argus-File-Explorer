#include "Functions.h"
#include "Drive.h"
#include "File.h"
#include "Directory.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <stack>

namespace fs = std::filesystem;

void searchPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& dirStack) {
    files.clear();
    directories.clear();
    try{
        for (const auto& dirEntry : fs::directory_iterator(Path, fs::directory_options::skip_permission_denied))
            dirEntry.is_directory() ? directories.push_back(Directory(dirEntry.path().generic_string())) : files.push_back(File(dirEntry.path().generic_string()));
        dirStack.push(Path);
    }
    catch (fs::filesystem_error) {
        std::cout << "Invalid Path\n";
        std::cin >> Path;
        searchPath(Path, directories, files, dirStack);
    }
}

void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& dirStack) {
    dirStack.pop();
    searchPath(dirStack.top(),directories,files,dirStack);
    dirStack.pop();
    //When i implement a GUI i should prevent from returning from the C: Dir
}

void debugPrint(std::vector<Directory>& directories, std::vector<File>& files) {
    std::cout << "Directories:\n";
    for (int i = 0;i < directories.size(); i++) 
        std::cout << directories[i].filePath << '\n';
    std::cout << "Files:\n";
    for (int i = 0;i < files.size(); i++) 
        std::cout << files[i].filePath << '\n';
}