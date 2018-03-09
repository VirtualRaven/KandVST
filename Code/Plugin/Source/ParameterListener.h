#ifndef PARAMETER_LISTENER
#define PARAMETER_LISTENER

#include "JuceHeader.h"
#include <mutex>
class ParameterListener : private AsyncUpdater
{
private:

	virtual void parametersChanged(std::vector<std::string>) = 0;
	// Inherited via AsyncUpdater
	virtual void handleAsyncUpdate() override;
	std::mutex __intListLock;
	std::vector<std::string> __pendingEventList;
public:
	ParameterListener();
	virtual ~ParameterListener();

	void callParameterChange(std::string);
	void callParameterChange(std::vector<std::string>);

};

#endif // !PARAMETER_LISTENER