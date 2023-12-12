#include "File.h"
#include <format>
#include <string>
namespace fs = std::filesystem;

File::File(std::string dirEntry)
{
    try {
        filePath = dirEntry;
        fileName = dirEntry.substr(dirEntry.find_last_of("//") + 1);
        getSize();
        fileNameLength = fileName.length();
    }
    catch (fs::filesystem_error) {
        filePath = "ERROR"; fileSizeKbs = 9;fileName = "ERROR";fileNameLength = 0;
    }

    //lastModificationTime = getLastModificationTime();
}

void File::newPath(std::string newPath) {
    newPath.append(filePath.substr(filePath.find_last_of("//")));
    fs::rename(filePath, newPath);
    filePath = newPath;
}

inline void File::getSize() { // in kBs
    fileSizeKbs = (fs::file_size(filePath))* 0.0009765625;
}

void File::copyFile(std::string newPath) {
    newPath.append(filePath.substr(filePath.find_last_of("//")));
    fs::copy_file(filePath, newPath,fs::copy_options::update_existing);
}

std::string File::getLastModificationTime(){
    std::time_t fileDate = std::chrono::system_clock::to_time_t(std::chrono::clock_cast<std::chrono::system_clock>(fs::last_write_time(filePath)));    
    std::tm* ptrTimeDetails = std::localtime(&fileDate);
    char buffer[32];
    std::strftime(buffer, 32, "%a, %m.%d.%Y %H:%M:%S", ptrTimeDetails);
    return buffer;
}