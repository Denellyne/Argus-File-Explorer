#pragma once
#include "Drive.h"
#include "Functions.h"
class File :
    public Drive
{
public:
    void newPath(std::string newPath);
    void getSize();
private:
    const double toMegabyte;
};


