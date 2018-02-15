#ifndef WHITE_NOISE_GENERATOR_H
#define WHITE_NOISE_GENERATOR_H


#include "../JuceLibraryCode/JuceHeader.h"
#include "IGenerator.h"
#include "EnvelopeGenerator.h"
class WhiteNoiseGenerator : public IGenerator
{
private:
	EnvelopeGenerator __envelope;
	int __note;
	double __angle;
	double __delta;
	bool __sustain; 
	int __sampleIndex;
	int __size;

	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);
	AudioBuffer<double> __noiseBuffer;
	Random __rand;

	double __NextSample();


public:
	WhiteNoiseGenerator(double sampleRate);
	~WhiteNoiseGenerator();

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

#endif //WHITE_NOISE_GENERATOR_H