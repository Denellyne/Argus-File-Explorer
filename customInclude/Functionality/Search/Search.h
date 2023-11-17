#pragma once
#include ".\Classes\File.h"
#include ".\Classes\Directory.h"
#include "Functionality/Debug/debug.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

void searchNewPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files,std::stack<std::string> &dirStack);
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack,std::string& userInputDirectory);

