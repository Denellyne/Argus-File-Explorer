#include "Functions.h"
#include "Drive.h"
#include "File.h"
#include "Directory.h"
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;

void setDirectoriesFiles(std::vector<Directory>& directories, std::vector<File>& files) {
    int i = 0, j = 0;
    
    for (const auto& dirEntry : fs::directory_iterator("C:\\", fs::directory_options::skip_permission_denied)){
        if (dirEntry.is_directory()) {
            directories.push_back(Directory());
            directories[i].filePath = dirEntry.path().generic_string();
            i++;
        }
        if (dirEntry.is_regular_file()) {
            files.push_back(File());
            files[j].filePath = dirEntry.path().generic_string();
            j++;
        }
    }
}
