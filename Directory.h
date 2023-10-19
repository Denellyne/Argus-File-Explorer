#pragma once
#include "Drive.h"
#include "Functions.h"
class Directory :
    public Drive
{
public:
    Directory();
    void newPath(std::string newPath);
    //Directory();
};

