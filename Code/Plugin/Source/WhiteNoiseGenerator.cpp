#include "WhiteNoiseGenerator.h"



WhiteNoiseGenerator::WhiteNoiseGenerator(double sampleRate) :
	IGenerator(sampleRate),
	__envelope(0, sampleRate),
	__note(0),
	__delta(0),
	__angle(0),
	__size(sampleRate),
	__noiseBuffer(1, __size),
	__rand(174594158), // Very random number
	__sampleIndex(0)
{
	// Generate wavetable
	for (int i = 0; i < __noiseBuffer.getNumSamples(); i++)
	{
		double next = (__rand.nextDouble() - 0.5f) * 2.0f;
		__noiseBuffer.setSample(0, i, next);
	}
}


WhiteNoiseGenerator::~WhiteNoiseGenerator()
{
}

void WhiteNoiseGenerator::ProccesNoteCommand(int note, uint8 vel, bool isOn)
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

double WhiteNoiseGenerator::__NextSample()
{
	if (__sampleIndex >= __size)
	{
		__sampleIndex = 0;
	}

	return __noiseBuffer.getSample(0, __sampleIndex++);
}

void WhiteNoiseGenerator::ProccessCommand(MidiMessage message)
{
}


template<typename T>
void WhiteNoiseGenerator::__RenderBlock(AudioBuffer<T>& buffer) {
	auto nextEvent = this->getNextEventOffset();
	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		
		if (i == nextEvent) {
			nextEvent= this->HandleEvent();
		}

		T samp = __NextSample();

		buffer.setSample(0, i, samp);
		buffer.setSample(1, i, samp);
	}
}

template void WhiteNoiseGenerator::__RenderBlock(AudioBuffer<double>& buffer);
template void WhiteNoiseGenerator::__RenderBlock(AudioBuffer<float>& buffer);
