#include "PipelineManager.h"



PipelineManager::PipelineManager(double rate, int maxBuffHint) :
 __sampleRate(rate),
 __maybeMaxBuff(maxBuffHint),
__log(),
pip(rate)
{
#ifdef  PIPE_MIDI_LOG
	__log.open(PIPE_MIDI_LOG, std::ofstream::trunc);
	__log << "Midi log (" << rate << " : " << maxBuffHint << ")" << std::endl;
#endif

}


PipelineManager::~PipelineManager()
{
	__log.close();
}

template<typename T>
void PipelineManager::genSamples(AudioBuffer<T>& buff, MidiBuffer & midiMessages)
{
	auto it = juce::MidiBuffer::Iterator(midiMessages);
	int pos;
	juce::MidiMessage tmp;
	while (it.getNextEvent(tmp, pos)) {
		if (tmp.isNoteOnOrOff()) {
			// finds or create pipeline
			tmp.getNoteNumber();
			__log << tmp.getNoteNumber() << ": " << tmp.getVelocity() << std::endl;
			pip.noteCommand(tmp.getNoteNumber(), tmp.getVelocity(), tmp.isNoteOn());
		}
	
		else {
		// broadcast
			__log <<tmp.getDescription() << std::endl;
		}
	}
	auto tmp_buff = pip.render_block<T>(buff.getNumSamples());
	buff.addFrom(0, 0, tmp_buff, 0, 0, buff.getNumSamples());
	buff.addFrom(1, 0, tmp_buff, 1, 0, buff.getNumSamples());
}

template void PipelineManager::genSamples(AudioBuffer<double>& buff, MidiBuffer & midiMessages);
template void PipelineManager::genSamples(AudioBuffer<float>& buff, MidiBuffer & midiMessages);