#pragma once

#include "Functionality/Functions.h"
#include <iostream>
class Directory{
public:
//Variables
    std::string filePath;

//Methods
    Directory(std::string dirEntry);
    void newPath(std::string newPath);
    void copyDirectory(std::string newPath);
};

