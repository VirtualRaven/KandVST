#ifndef DIST_H
#define DIST_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"

template<typename T>
class DistEffect :
	public IEffect<T>
{
public:
	DistEffect(double samplerate);
	~DistEffect();
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	virtual void ProccessCommand(MidiMessage message) override;
};
#endif