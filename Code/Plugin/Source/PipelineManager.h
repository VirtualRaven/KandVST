#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include <list>

template<typename T>
class PipelineManager
{
private:
	double __sampleRate;
	int   __maybeMaxBuff;
	std::vector<Pipeline<T>> pipList;
	
	ThreadPool pool;
public:
	PipelineManager(double rate, int maxBuffHint);
	~PipelineManager();	

void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages);


};

#endif //PIPELINE_MANAGER_H

