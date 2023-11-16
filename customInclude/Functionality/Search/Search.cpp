#include "Search.h"
#include "Functionality/Debug/debug.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

namespace fs = std::filesystem;

void searchPath(std::string Path, std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack);

//Public

void searchNewPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {

    if (directoryStack.top() != Path) {     
        try {
            searchPath(Path, directories, files, directoryStack);
            directoryStack.push(Path);
            Debug::print(directories, files);
        }
        catch (fs::filesystem_error) {
            std::cout << "Invalid Path\nInsert another path\n";
        }
    }
}
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    if (directoryStack.size() > 1 ) {
        directoryStack.pop();
        searchPath(directoryStack.top(), directories, files, directoryStack);
        Debug::print(directories, files);
    }
    else std::cout << "Invalid return\nStack only contains 1 element\n";
}

// Private

void searchPath(std::string Path, std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    files.clear();
    directories.clear();
    for (const auto& dirEntry : fs::directory_iterator(Path, fs::directory_options::skip_permission_denied))
        dirEntry.is_directory() ? directories.push_back(Directory(dirEntry.path().generic_string())) : files.push_back(File(dirEntry.path().generic_string()));
}

