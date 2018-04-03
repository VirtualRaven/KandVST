#include "PipelineManager.h"
#include "Pipeline.h"
#include <iostream>     
#include <iterator>     
#include <list>         
#include <thread>


template<typename T>
PipelineManager<T>::PipelineManager(double rate, int maxBuffHint,GLOBAL*global) :
	__sampleRate(rate),
	__maybeMaxBuff(maxBuffHint),
	__reverb(-1, rate, maxBuffHint, global),
	__filterLP(-1, rate, "FILTER_LP",global),
	__filterHP(-1, rate, "FILTER_HP",global),
	__delay(-1,rate,global)
{
	Global = global;
	__masterGain = Global->paramHandler->Get<AudioParameterFloat>(-1, "MASTER_GAIN");


	for (int i = 0; i < LFO_COUNT; i++) {
		lfos[i] = new LFO(maxBuffHint, i, rate,Global);
	}
	for (size_t i = 0; i < 16; i++)
	{
		pipList.emplace_back(rate,maxBuffHint, Global);
		
		pipBuff[i].setSize(2, maxBuffHint, false, true, false);
		pipBuff[i].applyGain(0);
	}
	
}

template<typename T>
PipelineManager<T>::~PipelineManager()
{
}

template<typename T>
void PipelineManager<T>::genSamples(AudioBuffer<T>& buff, MidiBuffer & midiMessages, AudioPlayHead::CurrentPositionInfo & posInfo)
{
	// Return if wavetables are not ready
	if (!wavetableRdy()) 	//Why do we do this check?
		return;

	auto buffLen = buff.getNumSamples();

	for (int i = 0; i < LFO_COUNT; i++) {
		lfos[i]->generate(buffLen, posInfo);
	}
	
	//Did they lie?
	if (__maybeMaxBuff < buffLen) {
		//Yes, and things will probably start to crumble down the line
		for (size_t i = 0; i < 16; i++)
		{
			pipBuff[i].setSize(2, buffLen);
			pipBuff[i].applyGain(0.0);
		}
		__maybeMaxBuff = buffLen;
	}
	

	typename std::vector<Pipeline<T>>::iterator pipIt;
	auto it = juce::MidiBuffer::Iterator(midiMessages);
	int pos;
	juce::MidiMessage tmp;
	it.setNextSamplePosition(0);
	while (it.getNextEvent(tmp, pos)) {
		if (tmp.isNoteOnOrOff()) {
			// finds or create pipeline

			bool foundPip = false;
			for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
				if (pipIt->getNoteNumber() == tmp.getNoteNumber()) {
					pipIt->midiCommand(tmp, pos);
					foundPip = true;
					break;
				}
			}

			if (foundPip)
				continue;

			for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
				if (!pipIt->isActive()) {
					pipIt->midiCommand(tmp, pos);
					break;
				}
			}
		}
		// Messages for all pipelines
		else if (tmp.isPitchWheel() || tmp.isSustainPedalOn() || tmp.isSustainPedalOff())
		{
			for (pipIt = pipList.begin(); pipIt != pipList.end(); pipIt++) {
				if (pipIt->isActive()) {
					// Regular midi command
					pipIt->midiCommand(tmp, pos);
				} else {
					// Force for inactive pipelines
					pipIt->forceMidiCommand(tmp);
				}
			}
		}
		else {
		}
	}
	//AudioBuffer<T> tmpBuff = AudioBuffer<T>(2, buff.getNumSamples());

	int buffCount = 0;
	for (pipIt = pipList.begin(); pipIt != pipList.end() - 1; pipIt++) {
		if (pipIt->isActive()) {
			
			//This assert makes sure that the buffer given to the pipeline is empty
			//If it goes off, someone has been writing outside of array bounds 
			//or we have failed to clear the buffer here in pipeline manager
			jassert(this->pipBuff[buffCount].getMagnitude(0, buffLen) < 0.01);
			
			std::function<void()> f = [pipIt, this, buffCount,buffLen]() {
				pipIt->render_block(this->pipBuff[buffCount],buffLen);
			};
			pool.addJob(f);
			buffCount++;
		}
	}
	//Run last job on this thread
	auto lastjob = (pipList.end() - 1);
	if (lastjob->isActive()) {
		lastjob->render_block(pipBuff[buffCount],buffLen);
		buffCount++;
	}
	while (pool.getNumJobs()>0);
	
	buff.clear();
	for (int i = 0; i < buffCount; i++)
	{
		buff.addFrom(0, 0, pipBuff[i], 0, 0, buffLen, *__masterGain);
		buff.addFrom(1, 0, pipBuff[i], 1, 0, buffLen, *__masterGain);
		pipBuff[i].clear(0, buffLen);
	}

	//Effects
	bool empty = buffCount == 0;

	__filterLP.RenderBlock(buff, buffLen, empty);
	__filterHP.RenderBlock(buff, buffLen, empty);
	__delay.RenderBlock(buff, buffLen, empty);

	__reverb.RenderBlock(buff, buffLen, false);
}


template<typename T>
void PipelineManager<T>::RegisterParameters(int ID, GLOBAL*Global)
{
	Global->paramHandler->RegisterFloat(ID, "MASTER_GAIN", "Master Volume", 0.0f, 1.0f, 1.0f);
}

template<typename T>
void PipelineManager<T>::Reset()
{
	for (auto& p : pipList) {
		p.Reset();
	}
}

template class PipelineManager<double>;
template class PipelineManager<float>;

