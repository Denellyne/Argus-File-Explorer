#include "Functions.h"
#include "File.h"

File::File()
{
}

void File::newPath(std::string newPath) {
    fs::rename(filePath, newPath);
    filePath = newPath;
    getSize();
}

void File::getSize() {
    fileSize = fs::file_size(filePath);
    fileSize = (fileSize > 1048576) ?  fileSize / 1048576 : fileSize;

}
