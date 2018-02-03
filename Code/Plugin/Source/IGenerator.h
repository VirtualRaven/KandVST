#ifndef GENERATOR_H
#define GENERATOR_H
#include "../JuceLibraryCode/JuceHeader.h"
class IGenerator
{

private:
	IGenerator();
protected:
	double __sampleRate;
public:
	IGenerator(double sampleRate) : __sampleRate(sampleRate) {};
	//Need to add RenderBlock to interface
	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //GENERATOR_H

