#include ".\Functionality\Search\Search.h"
#include <string>
#include <cstring>
#include <filesystem>

void driveIndex(std::vector<std::string>& drive);
void driveFilterFinder(std::vector<Directory>& directories, std::vector<File>& files,const std::string filter, const std::vector<std::string> &drive);