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
	for (size_t i = 0; i < 16; i++)
	{
		pipList.emplace_back(rate);
		//pipList.push_back(Pipeline(rate));
	}
	
}


PipelineManager::~PipelineManager()
{
}

template<typename T>
void PipelineManager::genSamples(AudioBuffer<T>& buff, MidiBuffer & midiMessages, AudioPlayHead::CurrentPositionInfo posInfo)
{
	std::vector<AudioBuffer<T>> pipBuff = std::vector<AudioBuffer<T>>();
	for (size_t i = 0; i < 16; i++)
	{
		pipBuff.push_back(AudioBuffer<T>(2, buff.getNumSamples()));
		pipBuff[i].applyGain(0.0);
	}
	std::vector<Pipeline>::iterator pipIt;
	auto it = juce::MidiBuffer::Iterator(midiMessages);
	int pos;
	juce::MidiMessage tmp;
	it.setNextSamplePosition(0);
	while (it.getNextEvent(tmp, pos)) {

		if (tmp.isNoteOnOrOff() && tmp.getNoteNumber() == 65) {
			if (tmp.isNoteOn())
				Global->log->Write("-----------------------------------------------------\n");
		}
		else
			Global->log->Write(String(pos) + String(" : ") + tmp.getDescription() + String("\n"));



		if (tmp.isNoteOnOrOff()) {
			// finds or create pipeline

			bool foundPip = false;
			for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
				if (pipIt->getNoteNumber() == tmp.getNoteNumber()) {
					pipIt->noteCommand(pos, tmp.getNoteNumber(), tmp.getVelocity(), tmp.isNoteOn());
					foundPip = true;
					break;
				}
			}

			if (foundPip)
				continue;

			for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
				if (!pipIt->isActive()) {
					pipIt->noteCommand(pos, tmp.getNoteNumber(), tmp.getVelocity(), tmp.isNoteOn());
					break;
				}
			}
		}

		else {
		}
	}
	//AudioBuffer<T> tmpBuff = AudioBuffer<T>(2, buff.getNumSamples());
	
	int buffCount = 0;
	for (pipIt = pipList.begin(); pipIt != pipList.end() - 1; pipIt++) {
		std::function<void()> f = [pipIt, &pipBuff, buffCount]() {
			pipIt->render_block(pipBuff[buffCount]);
		};

		pool.addJob(f);
		buffCount++;
	}
	//Run last job on this thread
	(pipList.end() - 1)->render_block(pipBuff[buffCount]);
	
	while (pool.getNumJobs()>0);
	for (auto b : pipBuff)
	{	
		buff.addFrom(0, 0, b, 0, 0, buff.getNumSamples());
		buff.addFrom(1, 0, b, 1, 0, buff.getNumSamples());
	}
	
}

template void PipelineManager::genSamples(AudioBuffer<double>& buff, MidiBuffer & midiMessage, AudioPlayHead::CurrentPositionInfo posInfo);
template void PipelineManager::genSamples(AudioBuffer<float>& buff, MidiBuffer & midiMessages, AudioPlayHead::CurrentPositionInfo posInfo);
