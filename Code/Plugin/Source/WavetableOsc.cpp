#include "WavetableOsc.h"

WavetableOsc::WavetableOsc(int ID,double sampleRate) :
	IGenerator(sampleRate),
	IVSTParameters(ID),
	__envelope(__ID,sampleRate),
	__note(0),
	__wavetable(tables[WAVE_TYPE::SAW]),
	__phase(0),
	__frequency(0),
	__lfo(120,sampleRate, __ID)
{
	__waveType = Global.paramHandler->Get<AudioParameterInt>(__ID, "WAVE_TYPE");
	__octave = Global.paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE");
	__offset = Global.paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET");
	__detune = Global.paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE");
	__lfo.amount = Global.paramHandler->Get<AudioParameterFloat>(__ID, "LFO_AMOUNT");
	__lfo.ratio = Global.paramHandler->Get<AudioParameterInt>(__ID, "LFO_RATIO");
	__lfo.isActive = Global.paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN");
	__lfo.bpm = Global.paramHandler->Get<AudioParameterInt>(__ID, "LFO_BPM");
}


WavetableOsc::~WavetableOsc()
{
}

void WavetableOsc::setWaveform(WAVE_TYPE t)
{
	if (t == WAVE_TYPE::__COUNT) return;
	__wavetable = tables[t];
}
/*void WavetableOsc::setOctave(int o)
{
	if (o > 3 || o < -3) return;
	__octave = o;
}*/

void WavetableOsc::ProccesNoteCommand(int note, uint8 vel, bool isOn)
{
	if (isOn)
	{
		__frequency = MidiMessage::getMidiNoteInHertz(note);
		__phase = 0.0;
		__note = note;
		__envelope.Reset();
		__sustain = true; //Right now we ignore sustain pedal
	}
	else if (!isOn && note == __note) {
		__sustain = false;
	}
	

}

void WavetableOsc::ProccessCommand(MidiMessage message)
{
}

void WavetableOsc::RegisterParameters(int ID)
{
	Global.paramHandler->RegisterInt(ID, "WAVE_TYPE", "Wave type", 0, 3, 0);
	Global.paramHandler->RegisterInt(ID, "OSC_OCTAVE", "Octave", -3, 3, 0);
	Global.paramHandler->RegisterInt(ID, "OSC_OFFSET", "Offset", -11, 11, 0);
	Global.paramHandler->RegisterFloat(ID, "OSC_DETUNE", "Detune", -1.0f, 1.0f, 0.0f);
	Global.paramHandler->RegisterBool(ID, "LFO_EN", "LFO", 0);
	Global.paramHandler->RegisterInt(ID, "LFO_BPM", "BPM", 30, 300, 120);
	Global.paramHandler->RegisterInt(ID, "LFO_RATIO", "LFO Ratio", 1, 10, 1); //TEMP!!!
	Global.paramHandler->RegisterFloat(ID, "LFO_AMOUNT", "LFO amount", -1.0, 1.0, 0.5);

}


template<typename T>
void WavetableOsc::__RenderBlock(AudioBuffer<T>& buffer) {
	
	
	setWaveform(toWAVE_TYPE(*__waveType));


	auto nextEvent = this->getNextEventOffset();
	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		if (i == nextEvent) {
			nextEvent = this->HandleEvent();
		}
		double tmpFreq = __frequency * pow(2.0, *__octave + (((*__offset) + (*__detune))/12.0));
		//tmpFreq *= pow(2.0, (*__offset) / 12.0);
		//tmpFreq *= pow(2.0, (*__detune) / 12.0);
		__lfo.apply(tmpFreq);
		double inc = __wavetable->getLength() * tmpFreq / __sampleRate;

		T samp = __wavetable->getSample(__phase, tmpFreq)* __envelope.GenerateNextStep(__sustain);
		__phase += inc;

		for(int j = 0; j < buffer.getNumChannels(); j++)
			buffer.setSample(j, i, samp);
	}
}

template void WavetableOsc::__RenderBlock(AudioBuffer<double>& buffer);
template void WavetableOsc::__RenderBlock(AudioBuffer<float>& buffer);

