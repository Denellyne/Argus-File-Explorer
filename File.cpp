#include "Functions.h"
#include "File.h"
void File::newPath(std::string newPath) {
    fs::rename(filePath, newPath);
    filePath = newPath;
    getSize();
}

void File::getSize() {
    fileSize = fs::file_size(filePath);
    if (fileSize < 1048576) {
    }
    else {
        fileSize = fileSize * toMegabyte;
    }
}