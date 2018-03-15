#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include <list>
#include "LFO.h"

template<typename T>
class PipelineManager
{
private:
	double __sampleRate;
	int   __maybeMaxBuff;
	std::vector<Pipeline<T>> pipList;
	AudioParameterFloat* __masterGain;
	ThreadPool pool;
public:
	PipelineManager(double rate, int maxBuffHint);
	~PipelineManager();	
	static void RegisterParameters(int ID);

void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages,
		AudioPlayHead::CurrentPositionInfo & posInfo);


};

#endif //PIPELINE_MANAGER_H

