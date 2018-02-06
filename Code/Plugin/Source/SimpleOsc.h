#ifndef SIMPLE_OSC_H
#define SIMPLE_OSC_H


#include "../JuceLibraryCode/JuceHeader.h"
#include "IGenerator.h"
#include "EnvelopeGenerator.h"
class SimpleOsc : public IGenerator
{
private:
	EnvelopeGenerator __envelope;
	int __note;
	double __angle;
	double __delta;
	bool __sustain; 

	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);

public:
	SimpleOsc(double sampleRate, ParameterHandler& paramHandler);
	~SimpleOsc();

	// Inherited via Generator
	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) override;
	virtual void ProccessCommand(MidiMessage message) override;

	virtual void RenderBlock(AudioBuffer<float>& buffer) override
	{
		__RenderBlock(buffer);
	}
	virtual void RenderBlock(AudioBuffer<double>& buffer) override
	{
		__RenderBlock(buffer);
	}
	
};

#endif //SIMPLE_OSC_H