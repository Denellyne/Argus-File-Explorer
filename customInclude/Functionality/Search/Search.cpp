#include "Search.h"
#include "Functionality/Debug/debug.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

namespace fs = std::filesystem;

void searchPath(std::string Path, std::vector<Directory>& directories, std::vector<File>& files);

//Public

void searchNewPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {

    if (directoryStack.top() != Path) {            
        searchPath(Path, directories, files);
        directoryStack.push(Path);
    }
}
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack,std::string &userInputDirectory,std::string &forwardPath) {
    forwardPath = directoryStack.top();
    directoryStack.pop();
    userInputDirectory = directoryStack.top();
    searchPath(directoryStack.top(), directories, files);
}

// Private

void searchPath(std::string Path, std::vector<Directory>& directories, std::vector<File>& files) {
    
    files.clear();
    directories.clear();
    try {
        for (const auto& dirEntry : fs::directory_iterator(Path, fs::directory_options::skip_permission_denied))
            fs::is_directory(dirEntry) ? directories.push_back(Directory(dirEntry.path().generic_string())) : files.push_back(File(dirEntry.path().generic_string()));
    }
    catch (fs::filesystem_error) { std::cout << "FsErro\n"; }
    catch (std::system_error) { std::cout << "SystemError\n"; }
}

