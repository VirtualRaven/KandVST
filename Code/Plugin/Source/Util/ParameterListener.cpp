#include "ParameterListener.h"



ParameterListener::ParameterListener()
{
}


ParameterListener::~ParameterListener()
{
}

void ParameterListener::handleAsyncUpdate()
{
	__intListLock.lock();
	std::vector<std::string> eventList(__pendingEventList);
	__pendingEventList.clear();
	__intListLock.unlock();
	parametersChanged(eventList);
}

void ParameterListener::callParameterChange(std::string s)
{
	__intListLock.lock();
	__pendingEventList.push_back(s);
	__intListLock.unlock();
	triggerAsyncUpdate();
}

void ParameterListener::callParameterChange(std::vector<std::string> sList)
{
	__intListLock.lock();
	for (std::string s : sList) 
	{
		sList.push_back(s);
	}
	__intListLock.unlock();
	triggerAsyncUpdate();
}
