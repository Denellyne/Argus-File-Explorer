#include "Functions.h"
#include "File.h"

File::File(std::string dirEntry)
{
    filePath = dirEntry;
    getSize();
}

void File::newPath(std::string newPath) {
    fs::rename(filePath, newPath);
    filePath = newPath;
    getSize();
}

void File::getSize() { // in kBs
    fileSize = fs::file_size(filePath);
    fileSize *= 0.0009765625;
}

void File::copyFile(std::string newPath) {
    fs::copy(filePath, newPath,fs::copy_options::update_existing);
}

