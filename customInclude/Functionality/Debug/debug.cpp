#pragma once
#include "debug.h"
#include "Functionality/Search/Search.h"
#include <chrono>
#include <iostream>

Debug::Timer::Timer() { m_StartTimepoint = std::chrono::high_resolution_clock::now(); };
Debug::Timer::~Timer() { Stop(); }
void Debug::Timer::Stop() {
    auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;

    if (duration > 300) std::cout << duration << "us ( " << ms << "ms )\n";
}

void Debug::print(std::vector<Directory>& directories, std::vector<File>& files)
{

    std::cout << "Directories:\n\n";
    for (int i = 0; i < directories.size(); i++)
        std::cout << directories[i].filePath << '\n';
    std::cout << "\nFiles:\n\n";
    for (int i = 0; i < files.size(); i++)
        std::cout << files[i].filePath.substr(files[i].filePath.find_last_of("//") + 1) << "\nSize: " << files[i].fileSizeKbs << " Kbs " << "\nLast modified time: " << files[i].lastModificationTime << '\n' << '\n';
}

