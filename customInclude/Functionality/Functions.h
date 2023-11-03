#pragma once

#include ".\Classes\File.h"
#include ".\Classes\Directory.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

void searchPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files,std::stack<std::string> &dirStack);
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack);


