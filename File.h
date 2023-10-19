#pragma once

#include "Drive.h"
#include "Functions.h"
class File :
    public Drive
{
public:
    File();
    uintmax_t fileSize;
    void newPath(std::string newPath);
    void getSize();

};


