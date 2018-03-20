#ifndef IEFFECT_H
#define IEFFECT_H
#include "../JuceLibraryCode/JuceHeader.h"

template<typename T>
class IEffect
{
private:
	IEffect();
protected:
	double __sampleRate;
public:
	IEffect(double sampleRate) : __sampleRate(sampleRate) {};
	virtual bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //IEFFECT_H