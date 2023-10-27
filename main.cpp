#include <iostream>
#include <vector>
#include "Functions.h"
#include <chrono>
#include <iostream>

int main() {
    std::vector<Directory> directories(0,Directory("C:\\")); // So it doesn't crash
    std::vector<File> files(0,File("C:\\Windows\\regedit.exe")); // So it doesn't crash
    setDirectoriesFiles("C:\\",directories, files);
    function1Test(directories, files);
    std::cout << files[2].fileSize << "kB\n";
    return 0;
}