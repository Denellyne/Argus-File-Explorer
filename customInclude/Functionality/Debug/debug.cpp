#pragma once
#include "debug.h"
#include "Functionality/Search/Search.h"

void Debug::print(std::vector<Directory>& directories, std::vector<File>& files)
{

    std::cout << "Directories:\n\n";
    for (int i = 0; i < directories.size(); i++)
        std::cout << directories[i].filePath << '\n';
    std::cout << "\nFiles:\n\n";
    for (int i = 0; i < files.size(); i++)
        std::cout << files[i].filePath.substr(files[i].filePath.find_last_of("//") + 1) << "\nSize: " << files[i].fileSizeKbs << " Kbs " << "\nLast modified time: " << files[i].lastModificationTime << '\n' << '\n';
}

