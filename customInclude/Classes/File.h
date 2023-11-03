#pragma once
#include "Functionality/Functions.h"
#include <iostream>
#include <filesystem>
#include <ctime>
#include <format>
class File
{
public:
    File(std::string dirEntry);   
    std::string filePath;
    uintmax_t fileSizeKbs;
    std::string lastModificationTime;

    void newPath(std::string newPath);
    inline void getSize();
    void copyFile(std::string newPath);
    std::string getLastModificationTime();
};


