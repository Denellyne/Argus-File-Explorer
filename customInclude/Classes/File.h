#pragma once
#include "Functions.h"
#include <iostream>
class File
{
public:
    std::string filePath;
    File(std::string dirEntry);    
    uintmax_t fileSize;
    void newPath(std::string newPath);
    inline void getSize();
    void copyFile(std::string newPath);
};


