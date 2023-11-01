#include "app.h"
#include ".\Functionality\Functions.h"

void fileExplorer(std::vector<Directory>& directories, std::vector<File>& files, std::stack<std::string>& directoryStack) {
    int choice = 0;
    std::string newPath;
    std::cin >> choice;
    switch (choice) {
    case 1:
        std::getline(std::cin >> std::ws, newPath);
        searchPath(newPath, directories, files, directoryStack);
        debugPrint(directories, files);
        break;
    case 2:
        returnPath(directories, files, directoryStack);
        break;
    }
}