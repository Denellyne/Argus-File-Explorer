#pragma once

#include <filesystem>
#include "Functions.h"

namespace fs = std::filesystem;
class Drive
{
public:   
    std::string filePath;
private:
    const std::filesystem::space_info driveBytes;
    uintmax_t driveSize;
};

