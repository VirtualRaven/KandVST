#ifndef WAVETABLEOSC_H
#define WAVETABLEOSC_H

#include "IGenerator.h"
#include "EnvelopeGenerator.h"
#include "IWavetable.h"
#include "IVSTParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"



class WavetableOsc : public IGenerator, public IVSTParameters
{
private:
	int __maxBuffHint;
	EnvelopeGenerator __envelope;
	int __note;
	double __frequency;
	double __phase;
	bool __sustain;

	AudioParameterInt* __waveType, *__octave,* __offset, *__overtone, *__pitchBendSens;
	AudioParameterFloat* __detune, *__sinAmp,* __sqAmp, *__sawAmp, *__triAmp,*__noiseAmp, *__panning;
	AudioParameterChoice* __lfofreq, *__lfoamp;
	float __pitchbend;
	template<typename T>
	bool __RenderBlock(AudioBuffer<T>& buffer,int len);

	double * __envBuff;

	//Random things
	std::vector<double> __noiseBuffer;
	Random __rand;
	size_t __rand_index;


public:
	WavetableOsc(int ID,double sampleRate,int __maxBuffHint);
	WavetableOsc(const WavetableOsc&) = delete;
	WavetableOsc(WavetableOsc&& ref);
	virtual ~WavetableOsc();

	// Inherited via Generator
	virtual void ProccessCommand(MidiMessage msg) override;
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
	void renderImage(Image* image);
	JUCE_LEAK_DETECTOR(WavetableOsc);
};

#endif //!WAVETABLEOSC_H

