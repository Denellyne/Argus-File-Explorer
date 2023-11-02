#include "Functionality/Functions.h"
#include <format>
#include "File.h"
#include <iostream>
namespace fs = std::filesystem;

File::File(std::string dirEntry)
{
    filePath = dirEntry;
    getSize();
}

void File::newPath(std::string newPath) {
    newPath.append(filePath.substr(filePath.find_last_of("//")));
    fs::rename(filePath, newPath);
    filePath = newPath;
}

inline void File::getSize() { // in kBs
    fileSize = (fs::file_size(filePath))* 0.0009765625;
}

void File::copyFile(std::string newPath) {
    newPath.append(filePath.substr(filePath.find_last_of("//")));
    fs::copy_file(filePath, newPath,fs::copy_options::update_existing);
}
