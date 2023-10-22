#pragma once

#include "Drive.h"
#include "File.h"
#include "Directory.h"
#include <filesystem>
#include <vector>
#include <iostream>

void setDirectoriesFiles(std::string Path,std::vector<Directory>& directories, std::vector<File>& files);
void function1Test(std::vector<Directory>& directories, std::vector<File>& files);
