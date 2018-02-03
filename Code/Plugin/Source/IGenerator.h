#ifndef IGENERATOR_H
#define IGENERATOR_H
#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterHandler.h"
class IGenerator
{

private:
	IGenerator();
protected:
	double __sampleRate;
	ParameterHandler* __paramHandler;
public:
	IGenerator(double sampleRate, ParameterHandler& paramHandler) : __sampleRate(sampleRate) { __paramHandler = &paramHandler; };
	virtual void RenderBlock(AudioBuffer<float>& buffer) = 0;
	virtual void RenderBlock(AudioBuffer<double>& buffer) = 0;

	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) = 0;
	virtual void ProccessCommand(MidiMessage message) = 0;
};

#endif //IGENERATOR_H

