#include "PipelineManager.h"
#include "Pipeline.h"
#include <iostream>     
#include <iterator>     
#include <list>         
#include <thread>


PipelineManager::PipelineManager(double rate, int maxBuffHint) :
	__sampleRate(rate),
	__maybeMaxBuff(maxBuffHint)
{
	ThreadPool &pool = ThreadPool();
	for (size_t i = 0; i < 16; i++)
	{
		pipList.push_back(Pipeline(rate));
	}
	
}


PipelineManager::~PipelineManager()
{
}

template<typename T>
void PipelineManager::genSamples(AudioBuffer<T>& buff, MidiBuffer & midiMessages)
{
	std::vector<AudioBuffer<T>> pipBuff = std::vector<AudioBuffer<T>>();
	for (size_t i = 0; i < 16; i++)
	{
		pipBuff.push_back(AudioBuffer<T>(2, buff.getNumSamples()));
		pipBuff[i].applyGain(0.0);
	}
	std::list<Pipeline>::iterator pipIt;
	auto it = juce::MidiBuffer::Iterator(midiMessages);
	int pos;
	juce::MidiMessage tmp;
	while (it.getNextEvent(tmp, pos)) {
		if (tmp.isNoteOnOrOff()) {
			// finds or create pipeline

			
			for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
				if (pipIt->isActive()) {
					continue;
				}
				else {	
					tmp.getNoteNumber();
					pipIt->noteCommand(pos,tmp.getNoteNumber(), tmp.getVelocity(), tmp.isNoteOn());
					break;
				}
			}
		}
	
		else {
		}
	}

	//AudioBuffer<T> tmpBuff = AudioBuffer<T>(2, buff.getNumSamples());
	
	int buffCount = 0;
	for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
		std::function<void()> f = [pipIt, &pipBuff,buffCount]() {
				pipIt->render_block(pipBuff[buffCount]);
		};

		pool.addJob(f);
		buffCount++;
		
	}
	while (pool.getNumJobs()>0);
	for (auto b : pipBuff)
	{	
		buff.addFrom(0, 0, b, 0, 0, buff.getNumSamples());
		buff.addFrom(1, 0, b, 1, 0, buff.getNumSamples());
	}
	
}

template void PipelineManager::genSamples(AudioBuffer<double>& buff, MidiBuffer & midiMessages);
template void PipelineManager::genSamples(AudioBuffer<float>& buff, MidiBuffer & midiMessages);
