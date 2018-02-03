#ifndef EFFECT_H
#define EFFECT_H
#include "../JuceLibraryCode/JuceHeader.h"
class IEffect
{
public:
	virtual void RenderBlock(AudioBuffer<float>& buffer) = 0;
	virtual void RenderBlock(AudioBuffer<double>& buffer) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //EFFECT_H