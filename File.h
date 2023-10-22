#pragma once
#include "Drive.h"
#include "Functions.h"
class File :
    public Drive
{
public:
    File(std::string dirEntry);
    uintmax_t fileSize = 0;
    void newPath(std::string newPath);
    void getSize();
    void copyFile(std::string newPath);
};


