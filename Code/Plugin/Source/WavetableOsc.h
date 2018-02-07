#ifndef WAVETABLEOSC_H
#define WAVETABLEOSC_H

#include "IGenerator.h"
#include "EnvelopeGenerator.h"
#include "IWavetable.h"


class WavetableOsc : public IGenerator
{
private:
	EnvelopeGenerator __envelope;
	int __note;
	double __frequency;
	double __phase;
	double __inc;
	bool __sustain;
	const IWavetable* __wavetable;

	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);

public:
	WavetableOsc(double sampleRate, ParameterHandler& paramHandler);
	~WavetableOsc();

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
	void setWaveform(WAVE_TYPE t);
};

#endif //!WAVETABLEOSC_H

