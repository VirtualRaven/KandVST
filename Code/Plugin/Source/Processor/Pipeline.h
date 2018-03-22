#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
#include "WavetableOsc.h"
#include "DelayEffect.h"
#include "LFO.h"
#include "FilterLP.h"
#include "FilterHP.h"
#include "DistEffect.h"

#include <tuple>
#include <array>

template<typename T>
class Pipeline 
{
	typedef std::tuple<IGenerator*, AudioParameterFloat*, AudioParameterBool*> OscTripple;
private:
	static const size_t __num_osc = 4;
	static const size_t __num_effects = 3;
	std::array<OscTripple, __num_osc> __oscs;
	std::array<IEffect<T>*, __num_osc*__num_effects> __effects;
	AudioBuffer<T> tmpBuff;
	double __rate;
	int __note;
	DelayEffect<T> __delay;
	bool __active;
	const int __maxBuffHint;

public:


	bool isActive();

	void midiCommand(MidiMessage msg, int offset);
	void forceMidiCommand(MidiMessage msg);


	Pipeline(double rate,int maxBuffHint);
	Pipeline(const Pipeline<T>&) = delete;
	Pipeline(Pipeline<T>&&);
	int getNoteNumber();
	void render_block(AudioBuffer<T>& buffer,int len);

	~Pipeline();
	static void RegisterParameters(int ID);
};

#endif