#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#define PIPE_MIDI_LOG "midi_log.txt"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include "ParameterHandler.h"

class PipelineManager
{
private:
	double __sampleRate;
	int   __maybeMaxBuff;
	std::ofstream  __log;
	Pipeline pip;
	ParameterHandler* __paramHandler;
public:
	PipelineManager(double rate, int maxBuffHint, ParameterHandler& paramHandler);
	~PipelineManager();
	

	template<typename T> void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages);


};

#endif //PIPELINE_MANAGER_H

