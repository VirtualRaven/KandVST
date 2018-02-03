#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
#include "SimpleOsc.h"
class Pipeline
{
	double __rate;
	SimpleOsc __osc;

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