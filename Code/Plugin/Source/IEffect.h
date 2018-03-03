#ifndef IEFFECT_H
#define IEFFECT_H
#include "../JuceLibraryCode/JuceHeader.h"
class IEffect
{
private:
	IEffect();
protected:
	double __sampleRate;
public:
	IEffect(double sampleRate) : __sampleRate(sampleRate) {};
	virtual bool RenderBlock(AudioBuffer<float>& buffer, int len, bool empty) = 0;
	virtual bool RenderBlock(AudioBuffer<double>& buffer, int len, bool empty) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //IEFFECT_H