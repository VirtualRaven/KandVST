#ifndef WAVETABLEOSC_H
#define WAVETABLEOSC_H

#include "IGenerator.h"
#include "EnvelopeGenerator.h"
#include "IWavetable.h"
#include "IVSTParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "LFO.h"

class WavetableOsc : public IGenerator, public IVSTParameters
{
private:
	EnvelopeGenerator __envelope;
	int __note;
	double __frequency;
	double __phase;
	bool __sustain;
	const IWavetable* __wavetable;

	AudioParameterInt* __waveType, *__octave,* __offset;
	AudioParameterFloat* __detune, *__sinAmp,* __sqAmp, *__sawAmp, *__triAmp,*__noiseAmp;
	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer,double gain);

	//Random things
	std::vector<double> __noiseBuffer;
	Random __rand;
	size_t __rand_index;

public:
	WavetableOsc(int ID,double sampleRate);
	virtual ~WavetableOsc();

	// Inherited via Generator
	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) override;
	virtual void ProccessCommand(MidiMessage message) override;
	static void RegisterParameters(int ID);
	virtual void RenderBlock(AudioBuffer<float>& buffer, double gain) override
	{
		__RenderBlock(buffer,gain);
	}
	virtual void RenderBlock(AudioBuffer<double>& buffer, double gain) override
	{
		__RenderBlock(buffer,gain);
	}

	virtual const char * name() const override {
		return "Wavetable osc";
	}
	void setWaveform(WAVE_TYPE t);
	void renderImage(Image* image,int width, int height);
	JUCE_LEAK_DETECTOR(WavetableOsc);
};

#endif //!WAVETABLEOSC_H

