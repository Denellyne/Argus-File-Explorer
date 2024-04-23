#include "drive.h"
#include ".\Functionality\Search\Search.h"
#include <string>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

void driveIndex(std::vector<std::string>& drive) {
    for (const auto& dirEntry : fs::recursive_directory_iterator("C:\\", fs::directory_options::skip_permission_denied)) {       
        try {
            drive.push_back(dirEntry.path().generic_string());
        }
        catch (std::system_error) {
            &fs::recursive_directory_iterator::increment;
        }
    }
}

void driveFilterFinder(std::vector<Directory>& directories, std::vector<File>& files,const std::string filter,const std::vector<std::string> &drive) {
    files.clear();
    directories.clear();

    for (const auto& dirEntry : drive) {
        try {
            if (dirEntry.substr(dirEntry.find_last_of("//") + 1).contains(filter)) fs::is_directory(dirEntry) ? directories.push_back(Directory(dirEntry)) : files.push_back(File(dirEntry));
        }
        catch (...) { break; }

    }

}