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
	const IWavetable* __wavetable;
	LFO __lfo;
	AudioParameterInt* __waveType, *__octave,* __offset;
	AudioParameterFloat* __detune, *__sinAmp,* __sqAmp, *__sawAmp, *__triAmp;
	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);

public:
	WavetableOsc(int ID,double sampleRate);
	~WavetableOsc();

	// Inherited via Generator
	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) override;
	virtual void ProccessCommand(MidiMessage message) override;
	static void RegisterParameters(int ID);
	virtual void RenderBlock(AudioBuffer<float>& buffer) override
	{
		__RenderBlock(buffer);
	}
	virtual void RenderBlock(AudioBuffer<double>& buffer) override
	{
		__RenderBlock(buffer);
	}
	void setWaveform(WAVE_TYPE t);
	void renderImage(Image* image,int width, int height);
};

#endif //!WAVETABLEOSC_H

