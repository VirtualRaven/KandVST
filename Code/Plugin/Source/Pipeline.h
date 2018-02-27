#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
#include "WavetableOsc.h"
#include "ExampleEffect.h"

#include <tuple>
#include <array>

class Pipeline 
{
	typedef std::tuple<IGenerator*, AudioParameterFloat*, AudioParameterBool*> OscTripple;
private:
	static const size_t __num_osc = 4;
	std::array<OscTripple, __num_osc> __oscs;

	double __rate;
	int __note;
	ExampleEffect __delay;
	bool __active;
public:


	bool isActive();
	void noteCommand(int offset,
					int note, 
					uint8 vel, 
					bool isOn =true );



	Pipeline(double rate);
	Pipeline(const Pipeline&) = delete;
	Pipeline(Pipeline&&);
	int getNoteNumber();
	template<typename T>
	void render_block(AudioBuffer<T>& buffer);

	~Pipeline();
	static void RegisterParameters(int ID);
};

#endif