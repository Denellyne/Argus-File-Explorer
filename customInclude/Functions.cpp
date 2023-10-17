#include "Functions.h"
#include "Drive.h"
#include "Directory.h"
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;

void searchPath(int& i) {

    for (const auto& dirEntry : fs::directory_iterator("C:\\"))

        if (dirEntry.is_directory()) {
            i++;
        }

}
void createPathVector(std::vector<Directory>& directories) {
    int i = 0;
    for (const auto& dirEntry : fs::directory_iterator("C:\\"))
        if (dirEntry.is_directory()) {
            directories.push_back(Directory());
            directories[i].filePath = dirEntry.path().generic_string();
            i++;
        }
}
