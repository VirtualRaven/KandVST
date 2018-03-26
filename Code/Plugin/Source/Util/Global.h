#ifndef GLOBAL_H
#define GLOBAL_H
#include "../JuceLibraryCode/JuceHeader.h"
class PresetManager;
class ParameterHandler;
class Log;

class GLOBAL {
public:
	PresetManager* presetManager;
	ParameterHandler* paramHandler;
	Log* log;
	GLOBAL(AudioProcessor* owner);
	~GLOBAL();
	JUCE_LEAK_DETECTOR(GLOBAL);
};

#endif // !GLOBAL_H
