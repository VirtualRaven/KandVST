#ifndef GLOBAL_H
#define GLOBAL_H

#include "PresetManager.h"
#include "ParameterHandler.h"
#include "Log.h"
#include "../JuceLibraryCode/JuceHeader.h"
class GLOBAL {
public:
	PresetManager* presetManager;
	ParameterHandler* paramHandler;
	Log* log;
	JUCE_LEAK_DETECTOR(GLOBAL);
	~GLOBAL()
	{
		delete paramHandler;
		delete log;
		delete presetManager;
		paramHandler = nullptr;
		log = nullptr;
		presetManager = nullptr;
	}
};
extern GLOBAL* Global;

#endif // !GLOBAL_H
