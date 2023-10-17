#pragma once
#include "Drive.h"
#include "Functions.h"
class Directory :
    public Drive
{
public:
    void newPath(std::string newPath);
};

