#pragma once
#include "Functionality/Functions.h"
#include <iostream>
class File
{
public:
    File(std::string dirEntry);   
    std::string filePath;
    uintmax_t fileSize;

    void newPath(std::string newPath);
    inline void getSize();
    void copyFile(std::string newPath);
};


