#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include <list>

class PipelineManager
{
private:
	double __sampleRate;
	int   __maybeMaxBuff;
	std::list<Pipeline> pipList;
	ThreadPool pool;
public:
	PipelineManager(double rate, int maxBuffHint);
	~PipelineManager();	

	template<typename T> void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages);


};

#endif //PIPELINE_MANAGER_H

