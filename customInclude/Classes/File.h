#pragma once
#include "Functionality/Functions.h"
#include <iostream>
#include <filesystem>
#include <ctime>
#include <format>
class File
{
public:
//Variables      
    std::string filePath;
    uintmax_t fileSizeKbs;
    std::string lastModificationTime;

//Methods
    File(std::string dirEntry);
    inline void getSize();
    void newPath(std::string newPath);   
    void copyFile(std::string newPath);
    std::string getLastModificationTime();
};


