#include "Log.h"



Log::Log(std::string filename):
	__file()
{
	__file.open(filename,std::ofstream::trunc);
}


Log::~Log()
{
	//__file.close();
}

template<typename T>
bool Log::Write(T text)
{
	if (!__file.is_open())
		return false;
	__fileLock.lock();
	__file << text;
	__file.flush();
	__fileLock.unlock();
	return true;
}

template bool Log::Write(char const *);
template bool Log::Write(String);
template bool Log::Write(std::string);
template bool Log::Write(int);
template bool Log::Write(float);