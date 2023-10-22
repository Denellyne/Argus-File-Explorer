#include <iostream>
#include <vector>
#include "Functions.h"
#include <chrono>
#include <iostream>
class Timer
{
public:
    Timer() {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        Stop();
    }
    void Stop() {
        auto endTimePoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;

        std::cout << duration << "us ( " << ms << "ms )\n";
    }

private:
    std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimepoint;
};

int main() {
    Timer time;
    std::vector<Directory> directories(0,Directory("C:\\"));
    std::vector<File> files(0,File("C:\\Windows\\regedit.exe")); // So it doesn't crash
    setDirectoriesFiles("C:\\",directories, files);
    function1Test(directories, files);
    std::cout << files[2].fileSize << "kB\n";
    return 0;
}