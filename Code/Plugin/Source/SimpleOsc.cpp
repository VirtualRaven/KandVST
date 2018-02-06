#include "SimpleOsc.h"



SimpleOsc::SimpleOsc(double sampleRate, ParameterHandler& paramHandler) :
	IGenerator(sampleRate,paramHandler),
	__envelope(sampleRate,paramHandler),
	__note(0),
	__delta(0),
	__angle(0)
{

}


SimpleOsc::~SimpleOsc()
{
}

void SimpleOsc::ProccesNoteCommand(int note, uint8 vel, bool isOn)
{
	if (isOn) 
	{
		__angle = 0;
		__delta = MidiMessage::getMidiNoteInHertz(note) * 2 * juce::MathConstants<double>().pi  / __sampleRate;
		__note = note;
		__envelope.Reset();
	}

	__sustain = isOn; //Right now we ignore sustain pedal

}

void SimpleOsc::ProccessCommand(MidiMessage message)
{
}


template<typename T>
void SimpleOsc::__RenderBlock(AudioBuffer<T>& buffer) {
	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		
		__angle =fmod(__angle + __delta,2*juce::MathConstants<T>().pi);
		T samp = sin(__angle)*__envelope.GenerateNextStep(__sustain);

		buffer.setSample(0, i, samp);
		buffer.setSample(1, i, samp);
	}
}

template void SimpleOsc::__RenderBlock(AudioBuffer<double>& buffer);
template void SimpleOsc::__RenderBlock(AudioBuffer<float>& buffer);
