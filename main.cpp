#include <vector>
#include <stack>
#include ".\Main\app.h"

int main() {
    bool programRunning = true;

    std::vector<Directory> directories(0, Directory("C:\\"));
    std::vector<File> files(0, File("fileExplorer.exe"));
    std::stack<std::string> directoryStack;
    directoryStack.push("C:\\");

    while (programRunning) fileExplorer(directories, files, directoryStack);

    return 0;
}



