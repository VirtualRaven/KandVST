#ifndef IEFFECT_H
#define IEFFECT_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"
class IEffect
{
private:
	IEffect();
protected:
	double __sampleRate;
	ParameterHandler* __paramHandler;
public:
	IEffect(double sampleRate, ParameterHandler& paramHandler) : __sampleRate(sampleRate) { __paramHandler = &paramHandler; };
	virtual void RenderBlock(AudioBuffer<float>& buffer) = 0;
	virtual void RenderBlock(AudioBuffer<double>& buffer) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //IEFFECT_H