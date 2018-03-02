#ifndef WAVETABLEOSC_H
#define WAVETABLEOSC_H

#include "IGenerator.h"
#include "EnvelopeGenerator.h"
#include "IWavetable.h"
#include "IVSTParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOsc.h"


class WavetableOsc : public IGenerator, public IVSTParameters
{
private:
	EnvelopeGenerator __envelope;
	int __note;
	double __frequency;
	double __phase;
	//double __inc;
	bool __sustain;
	LFO __lfo;
	AudioParameterInt* __waveType, *__octave,* __offset;
	AudioParameterFloat* __detune, *__sinAmp,* __sqAmp, *__sawAmp, *__triAmp,*__noiseAmp;
	float __pitchbend;
	template<typename T>
	bool __RenderBlock(AudioBuffer<T>& buffer,int len);

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
	virtual bool RenderBlock(AudioBuffer<float>& buffer,int len) override
	{
		return __RenderBlock(buffer, len);
	}
	virtual bool RenderBlock(AudioBuffer<double>& buffer, int len) override
	{
		return __RenderBlock(buffer, len);
	}

	virtual const char * name() const override {
		return "Wavetable osc";
	}
	void renderImage(Image* image,int width, int height);
	JUCE_LEAK_DETECTOR(WavetableOsc);
};

#endif //!WAVETABLEOSC_H

