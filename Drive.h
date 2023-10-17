#pragma once
#include <filesystem>
#include "Functions.h"
namespace fs = std::filesystem;
class Drive
{
public:
    const std::filesystem::space_info driveBytes;
    uintmax_t driveSize;
    std::string filePath;
    uintmax_t fileSize;
};

