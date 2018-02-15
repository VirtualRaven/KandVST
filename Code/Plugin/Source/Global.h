#ifndef GLOBAL_H
#define GLOBAL_H

#include "Log.h"
#include "../JuceLibraryCode/JuceHeader.h"
class ParameterHandler;
class GLOBAL {
public:
	ParameterHandler* paramHandler;
	Log* log;
	JUCE_LEAK_DETECTOR(GLOBAL);
	~GLOBAL()
	{
		delete paramHandler;
		delete log;
		paramHandler = nullptr;
		log = nullptr;
	}
};
extern GLOBAL* Global;

#endif // !GLOBAL_H
