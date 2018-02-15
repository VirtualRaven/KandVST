#ifndef OSCILLATOR_MIXER
#define OSCILLATOR_MIXER

#include "IGenerator.h"
#include "IVSTParameters.h"
#include "Global.h"
#include "WavetableOsc.h"
class OscillatorMixer: public IGenerator, private IVSTParameters
{
private:
	bool __active;
	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);
	std::vector<std::tuple<WavetableOsc, AudioParameterFloat*,AudioParameterBool*>>  __oscillators;
public:
	OscillatorMixer(int ID,double sampleRate);
	~OscillatorMixer();

	bool isActive();
	// Inherited via IGenerator
	virtual void RenderBlock(AudioBuffer<float>& buffer) override
	{
		__RenderBlock(buffer);
	}
	virtual void RenderBlock(AudioBuffer<double>& buffer) override
	{
		__RenderBlock(buffer);
	}
	virtual void ProccesNoteCommand(int note, uint8 vel, bool isOn) override;
	virtual void AddNoteCommand(int offset, int note, uint8 vel, bool isOn) override;
	virtual void ProccessCommand(MidiMessage message) override;

	static void RegisterParameters(int ID);
};

#endif // !OSCILLATOR_MIXER

