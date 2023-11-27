#include "drive.h"
#include ".\Functionality\Search\Search.h"
#include <string>
#include <cstring>
#include <filesystem>


namespace fs = std::filesystem;

void driveIndex(std::vector<std::string>& drive) {

    for (auto& dirEntry : fs::recursive_directory_iterator("C:\\", fs::directory_options::skip_permission_denied)) {
        try {
            drive.push_back(dirEntry.path().generic_string());
        }
        catch (std::system_error) {
            &fs::recursive_directory_iterator::increment;
        }
    }
}

void driveFilterFinder(std::vector<Directory>& directories, std::vector<File>& files,std::string filter,std::vector<std::string> drive) { //For the time being
    files.clear();
    directories.clear();
    for (size_t i = 0;i < drive.size();i++) {
        std::string path = drive[i].substr(drive[i].find_last_of("//") + 1);
        if (strstr(path.c_str(), filter.c_str())) {
            fs::is_directory(drive[i]) ? directories.push_back(Directory(drive[i])) : files.push_back(File(drive[i]));
        }
    }

}