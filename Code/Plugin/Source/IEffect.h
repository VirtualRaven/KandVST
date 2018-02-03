#ifndef EFFECT_H
#define EFFECT_H
#include "../JuceLibraryCode/JuceHeader.h"
class IEffect
{
private:
	IEffect();
protected:
	double __sampleRate;
public:
	IEffect(double sampleRate) : __sampleRate(sampleRate) {};
	virtual void RenderBlock(AudioBuffer<float>& buffer) = 0;
	virtual void RenderBlock(AudioBuffer<double>& buffer) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //EFFECT_H