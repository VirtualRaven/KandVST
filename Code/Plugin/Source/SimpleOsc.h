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
public:
	SimpleOsc(double sampleRate);
	~SimpleOsc();

	// Inherited via Generator
	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) override;
	virtual void ProccessCommand(MidiMessage message) override;

	template<typename T>
	void RenderBlock(AudioBuffer<T>& buffer);
};

#endif //SIMPLE_OSC_H