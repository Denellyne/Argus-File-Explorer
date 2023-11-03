#include "Functions.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

namespace fs = std::filesystem;

void debugPrint(std::vector<Directory>& directories, std::vector<File>& files);

void searchPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {

    if (directoryStack.top() != Path) {
        files.clear();
        directories.clear();
        try {
            for (const auto& dirEntry : fs::directory_iterator(Path, fs::directory_options::skip_permission_denied))
                dirEntry.is_directory() ? directories.push_back(Directory(dirEntry.path().generic_string())) : files.push_back(File(dirEntry.path().generic_string()));  
            directoryStack.push(Path);
            debugPrint(directories, files);
        }
        catch (fs::filesystem_error) {
            std::cout << "Invalid Path\nInsert another path\n";
            std::getline(std::cin >> std::ws, Path);
            searchPath(Path, directories, files, directoryStack);
        }
    }
}
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    if (directoryStack.size() > 1 ) {
        directoryStack.pop();
        files.clear();
        directories.clear();
        for (const auto& dirEntry : fs::directory_iterator(directoryStack.top(), fs::directory_options::skip_permission_denied))
            dirEntry.is_directory() ? directories.push_back(Directory(dirEntry.path().generic_string())) : files.push_back(File(dirEntry.path().generic_string()));  
        debugPrint(directories, files);
    }
    else std::cout << "Invalid return\nStack only contains 1 element\n";
}

//Debug


void debugPrint(std::vector<Directory>& directories, std::vector<File>& files) {
    std::cout << "Directories:\n\n";
    for (int i = 0;i < directories.size(); i++) 
        std::cout << directories[i].filePath << '\n';
    std::cout << "Files:\n\n";
    for (int i = 0;i < files.size(); i++) 
        std::cout << files[i].filePath.substr(files[i].filePath.find_last_of("//") + 1) << "\nSize: " << files[i].fileSizeKbs << " Kbs " << "\nLast modified time: " << files[i].lastModificationTime << '\n' << '\n';
}
