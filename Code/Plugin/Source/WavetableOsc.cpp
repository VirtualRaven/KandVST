#include "WavetableOsc.h"

WavetableOsc::WavetableOsc(int ID,double sampleRate) :
	IGenerator(sampleRate),
	IVSTParameters(ID),
	__envelope(__ID,sampleRate),
	__note(0),
	__wavetable(tables[WAVE_TYPE::SAW]),
	__phase(0),
	__frequency(0)
{
	__waveType = Global.paramHandler->Get<AudioParameterInt>(__ID, "WAVE_TYPE");
}


WavetableOsc::~WavetableOsc()
{
}

void WavetableOsc::setWaveform(WAVE_TYPE t)
{
	if (t == WAVE_TYPE::__COUNT) return;
	__wavetable = tables[t];
}

void WavetableOsc::ProccesNoteCommand(int note, uint8 vel, bool isOn)
{
	if (isOn)
	{
		__frequency = MidiMessage::getMidiNoteInHertz(note);
		__phase = 0.0;
		__inc = __wavetable->getLength() * __frequency / __sampleRate;
		__note = note;
		__envelope.Reset();

	}

	__sustain = isOn; //Right now we ignore sustain pedal

}

void WavetableOsc::ProccessCommand(MidiMessage message)
{
}

void WavetableOsc::RegisterParameters(int ID)
{
	Global.paramHandler->RegisterInt(ID, "WAVE_TYPE", "Wave type", 0, 3, 0);
	Global.paramHandler->RegisterBool(ID, "WAVE_TYPEs", "Wave types", true);
}


template<typename T>
void WavetableOsc::__RenderBlock(AudioBuffer<T>& buffer) {
	setWaveform(toWAVE_TYPE(*__waveType));
	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		T samp = __wavetable->getSample(__phase, __frequency)*__envelope.GenerateNextStep(__sustain);
		__phase += __inc;

		buffer.setSample(0, i, samp);
		buffer.setSample(1, i, samp);
	}
}

template void WavetableOsc::__RenderBlock(AudioBuffer<double>& buffer);
template void WavetableOsc::__RenderBlock(AudioBuffer<float>& buffer);

