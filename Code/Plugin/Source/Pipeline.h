#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
#include "WavetableOsc.h"
#include "ExampleEffect.h"
class Pipeline 
{
	
private:
	double __rate;
	WavetableOsc __osc;
	ExampleEffect __delay;
	bool _active;
public:

	bool isActive();
	void noteCommand(int note, 
					uint8 vel, 
					bool isOn =true );



	Pipeline(double rate);

	template<typename T>
	void render_block(AudioBuffer<T>& buffer);

	~Pipeline();
};

#endif