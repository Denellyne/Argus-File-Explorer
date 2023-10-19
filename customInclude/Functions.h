#pragma once

#include "Drive.h"
#include "File.h"
#include "Directory.h"
#include <filesystem>
#include <vector>


void setDirectoriesFiles(std::vector<Directory>& directories, std::vector<File>& files);
