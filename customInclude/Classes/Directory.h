#pragma once

#include "Functions.h"
#include <iostream>
class Directory{
public:
    std::string filePath;
    Directory(std::string dirEntry);
    void newPath(std::string newPath);
    void copyDirectory(std::string newPath);
};

