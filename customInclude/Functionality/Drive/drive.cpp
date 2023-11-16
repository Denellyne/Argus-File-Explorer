#include "drive.h"
#include <filesystem>

namespace fs = std::filesystem;

void driveIndex(std::vector<std::string>& drive) {

    for (auto& dirEntry : fs::recursive_directory_iterator("C:\\", fs::directory_options::skip_permission_denied)) {
        try {
            drive.push_back(dirEntry.path().string());
        }
        catch (std::system_error) {
            &fs::recursive_directory_iterator::increment;
        }
    }
}