#include "Functions.h"
#include "Directory.h"


Directory::Directory()
{
}

void Directory::newPath(std::string newPath) {   // Renames // Changes the directory of the file
    fs::rename(filePath, newPath);
    filePath = newPath;
}
