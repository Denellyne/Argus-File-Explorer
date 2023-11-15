#pragma once
#include ".\Functionality\Search\Search.h"

void fileExplorer(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack);
int GUI();
