#pragma once

#include "Drive.h"
#include "File.h"
#include "Directory.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <stack>

void searchPath(std::string Path,std::vector<Directory>& directories, std::vector<File>& files,std::stack<std::string> &dirStack);
void returnPath(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& dirStack);
void function1Test(std::vector<Directory>& directories, std::vector<File>& files);
