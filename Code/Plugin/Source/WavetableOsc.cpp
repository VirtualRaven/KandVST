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
	__waveType = Global->paramHandler->Get<AudioParameterInt>(__ID, "WAVE_TYPE");
	__octave = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE");
	__offset = Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET");
	__detune = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE");

	__sinAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SINE");
	__sqAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SQUARE");
	__sawAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SAW");
	__triAmp = Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_TRI");
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
		//__phase = 0.0;
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
	Global->paramHandler->RegisterInt(ID, "WAVE_TYPE", "Wave type", 0, 3, 0);
	Global->paramHandler->RegisterInt(ID, "OSC_OCTAVE", "Octave", -3, 3, 0);
	Global->paramHandler->RegisterInt(ID, "OSC_OFFSET", "Offset", -11, 11, 0);
	Global->paramHandler->RegisterFloat(ID, "OSC_DETUNE", "Detune", -1.0f, 1.0f, 0.0f);

	Global->paramHandler->RegisterFloat(ID, "OSC_SINE", "Sine", 0.0f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_SQUARE", "Square", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_SAW", "Saw", 0.0f, 1.0f, 0.0f);
	Global->paramHandler->RegisterFloat(ID, "OSC_TRI", "Tri", 0.0f, 1.0f, 0.0f);

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

		__lfo.apply(tmpFreq);
		double inc = __wavetable->getLength() * tmpFreq / __sampleRate;

		T envStep = __envelope.GenerateNextStep(__sustain);;

		T samp = tables[WAVE_TYPE::SINE]->getSample(__phase, tmpFreq)*envStep* (*__sinAmp);
		samp += tables[WAVE_TYPE::SQUARE]->getSample(__phase, tmpFreq)*envStep* (*__sqAmp);
		samp += tables[WAVE_TYPE::SAW]->getSample(__phase, tmpFreq)*envStep* (*__sawAmp);
		samp += tables[WAVE_TYPE::TRI]->getSample(__phase, tmpFreq)*envStep* (*__triAmp);

		__phase += inc;

		for(int j = 0; j < buffer.getNumChannels(); j++)
			buffer.setSample(j, i, samp);
	}
}

template void WavetableOsc::__RenderBlock(AudioBuffer<double>& buffer);
template void WavetableOsc::__RenderBlock(AudioBuffer<float>& buffer);

