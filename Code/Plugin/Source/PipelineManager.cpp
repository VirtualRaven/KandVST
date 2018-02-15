#include "PipelineManager.h"
#include "Pipeline.h"
#include <iostream>     
#include <iterator>     
#include <list>         
#include <thread>


PipelineManager::PipelineManager(double rate, int maxBuffHint) :
	__sampleRate(rate),
	__maybeMaxBuff(maxBuffHint),
	__log(),
	pip(rate),
	pip2(rate)
	

{
#ifdef  PIPE_MIDI_LOG
	__log.open(PIPE_MIDI_LOG, std::ofstream::trunc);
	__log << "Midi log (" << rate << " : " << maxBuffHint << ")" << std::endl;
#endif
	ThreadPool &pool = ThreadPool();
	for (size_t i = 0; i < 16; i++)
	{

		pipList.push_back(Pipeline(rate));
	}
	
}


PipelineManager::~PipelineManager()
{
	__log.close();
}

template<typename T>
void PipelineManager::genSamples(AudioBuffer<T>& buff, MidiBuffer & midiMessages)
{
	std::vector<AudioBuffer<T>> pipBuff = std::vector<AudioBuffer<T>>();
	for (size_t i = 0; i < 16; i++)
	{
		pipBuff.push_back(AudioBuffer<T>(2, buff.getNumSamples()));
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
					__log << tmp.getNoteNumber() << ": " << tmp.getVelocity() << std::endl;
					pipIt->noteCommand(tmp.getNoteNumber(), tmp.getVelocity(), tmp.isNoteOn());
				}
			}
		}
	
		else {
		// broadcast
			__log <<tmp.getDescription() << std::endl;
		}
	}

	//AudioBuffer<T> tmpBuff = AudioBuffer<T>(2, buff.getNumSamples());
	
	int buffCount = 0;
	for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
		std::function<void()> f = [pipIt, &pipBuff,buffCount]() {
			try {
				pipIt->render_block(pipBuff[buffCount]);
			}
			catch (...) {
				// We're fucked
				Global.log->Write("We're fucked!!!");
			}
	};
		pool.addJob(f);
		buffCount++;
		
	}
	while (pool.getNumJobs());
	for (auto b : pipBuff)
	{	
		
		buff.addFrom(0, 0, b, 0, 0, buff.getNumSamples());
		buff.addFrom(1, 0, b, 1, 0, buff.getNumSamples());
	}
	
	
	
}

template void PipelineManager::genSamples(AudioBuffer<double>& buff, MidiBuffer & midiMessages);
template void PipelineManager::genSamples(AudioBuffer<float>& buff, MidiBuffer & midiMessages);
