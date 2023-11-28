#pragma once
#include "Functionality/Search/Search.h"

namespace Debug {
	class Timer {
	private:
		std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimepoint;
	public:
		Timer();
		void Stop();
		~Timer();
	};
	

	void print(std::vector<Directory>& directories, std::vector<File>& files);

}

