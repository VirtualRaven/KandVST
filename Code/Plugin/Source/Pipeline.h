#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
#include "WavetableOsc.h"
#include "ExampleEffect.h"
#include "FilterLP.h"
#include "FilterHP.h"

#include <tuple>
#include <array>

template<typename T>
class Pipeline 
{
	typedef std::tuple<IGenerator*, AudioParameterFloat*, AudioParameterBool*> OscTripple;
private:
	static const size_t __num_osc = 4;
	static const size_t __num_effects = 2;
	std::array<OscTripple, __num_osc> __oscs;
	std::array<IEffect*, __num_osc*__num_effects> __effects;
	AudioBuffer<T> tmpBuff;
	double __rate;
	int __note;
	ExampleEffect __delay;
	bool __active;
	const int __maxBuffHint;

public:


	bool isActive();
	void noteCommand(int offset,
					int note, 
					uint8 vel, 
					bool isOn =true );



	Pipeline(double rate,int maxBuffHint);
	Pipeline(const Pipeline<T>&) = delete;
	Pipeline(Pipeline<T>&&);
	int getNoteNumber();
	void render_block(AudioBuffer<T>& buffer);

	~Pipeline();
	static void RegisterParameters(int ID);
};

#endif