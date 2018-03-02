#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include <list>
#include "LFO.h"

class PipelineManager
{
private:
	double __sampleRate;
	int   __maybeMaxBuff;
	std::vector<Pipeline> pipList;
	ThreadPool pool;
public:
	PipelineManager(double rate, int maxBuffHint);
	~PipelineManager();	
	
	template<typename T> void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages,
		AudioPlayHead::CurrentPositionInfo & posInfo);


};

#endif //PIPELINE_MANAGER_H

