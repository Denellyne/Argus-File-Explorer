#pragma once
#include "Drive.h"
#include "Functions.h"
class Directory :
    public Drive
{
public:
    Directory(std::string dirEntry);
    void newPath(std::string newPath);
    void copyDirectory(std::string newPath);
};

