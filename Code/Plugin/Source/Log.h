#ifndef LOG_H
#define LOG_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <mutex>
#include <sstream>
#include <iostream>
#include <fstream>
class Log 
{
private:
	
	std::mutex __fileLock;
	std::ofstream __file;
public:
	template <typename T>
	bool Write(T text);

	Log(std::string filename);
	~Log();
	JUCE_LEAK_DETECTOR(Log);
};

#endif


