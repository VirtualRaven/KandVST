#ifndef EFFECT_H
#define EFFECT_H
#include "../JuceLibraryCode/JuceHeader.h"
class IEffect
{
public:
	//Add RenderBlock to interface
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //EFFECT_H