#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include <list>
#include <array>
#include "LFO.h"
#include "FilterHP.h"
#include "FilterLP.h"

template<typename T>
class PipelineManager
{
private:
	static const size_t NUM_PIPELINES = 16;
	double __sampleRate;
	int   __maybeMaxBuff;
	std::vector<Pipeline<T>> pipList;
	std::array<AudioBuffer<T>,NUM_PIPELINES> pipBuff;
	AudioParameterFloat* __masterGain;
	ThreadPool pool;
	FilterHP<T> __filterHP;
	FilterLP<T> __filterLP;
	DelayEffect<T>__delay;
public:
	PipelineManager(double rate, int maxBuffHint,GLOBAL*global);
	~PipelineManager();	
	static void RegisterParameters(int ID, GLOBAL*Global);
	void Reset();
	GLOBAL * Global;

void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages,
		AudioPlayHead::CurrentPositionInfo & posInfo);


};

#endif //PIPELINE_MANAGER_H

