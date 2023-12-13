#pragma once
#include <string>
#include <filesystem>
#include <ctime>
#include <format>
class File
{
public:
//Variables      
    std::string filePath;
    std::string fileName;
    size_t fileNameLength;
    uintmax_t fileSizeKbs;
    std::string lastModificationTime;
    std::string fileExtension;

//Methods
    File(std::string dirEntry);
    inline void getSize();
    void newPath(std::string newPath);   
    void copyFile(std::string newPath);
    std::string getLastModificationTime();
};


