#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
class Pipeline
{
	double __rate;
	int __note; 
	double __freq;
	uint8 __vel;
	bool __pressed;
	bool __active;
	double prev_angle;
	double amplitude;
	EnvelopeGenerator __env;

public:


	bool isActive();
	void noteCommand(int note, 
					uint8 vel, 
					bool isOn =true );



	Pipeline(double rate);

	template<typename T>
	AudioBuffer<T> render_block(int sampels);

	~Pipeline();
};

#endif