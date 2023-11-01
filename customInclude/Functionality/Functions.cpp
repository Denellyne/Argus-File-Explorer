#include "Functions.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

namespace fs = std::filesystem;

void searchPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    files.clear();
    directories.clear();
    try{
        for (const auto& dirEntry : fs::directory_iterator(Path, fs::directory_options::skip_permission_denied))
            dirEntry.is_directory() ? directories.push_back(Directory(dirEntry.path().generic_string())) : files.push_back(File(dirEntry.path().generic_string()));
        if (directoryStack.top() != Path) directoryStack.push(Path);
    }
    catch (fs::filesystem_error) {
        std::cout << "Invalid Path\nInsert another path\n";
        std::cin >> Path;
        searchPath(Path, directories, files, directoryStack);
    }
}
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    if (directoryStack.size() < 1 ) {
        directoryStack.pop();
        searchPath(directoryStack.top(), directories, files, directoryStack);
        debugPrint(directories, files);
        directoryStack.pop();
    }
    else {
        std::cout << "Invalid return\nStack only contains 1 element\n";
    }
}




void debugPrint(std::vector<Directory>& directories, std::vector<File>& files) {
    std::cout << "Directories:\n";
    for (int i = 0;i < directories.size(); i++) 
        std::cout << directories[i].filePath << '\n';
    std::cout << "Files:\n";
    for (int i = 0;i < files.size(); i++) 
        std::cout << files[i].filePath << " " << files[i].fileSize << '\n';
}
