#include "Functions.h"
#include "Directory.h"


Directory::Directory(std::string dirEntry)
{
    filePath = dirEntry;

}

void Directory::newPath(std::string newPath) {   // Renames // Changes the directory of the file
    fs::rename(filePath, newPath);
    filePath = newPath;
}

void Directory::copyDirectory(std::string newPath) {
    fs::copy(filePath, newPath, fs::copy_options::recursive);
}
