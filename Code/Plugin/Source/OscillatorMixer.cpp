#include "OscillatorMixer.h"



OscillatorMixer::OscillatorMixer(int ID, double sampleRate):
	IGenerator(sampleRate),
	IVSTParameters(ID),
	__oscillators()
{
	for (size_t i = 0; i < 4; i++)
	{
		__oscillators.push_back(std::make_tuple(WavetableOsc(i, sampleRate), Global->paramHandler->Get<AudioParameterFloat>(i, "OSC_MIX_AMP"), Global->paramHandler->Get<AudioParameterBool>(i, "OSC_MIX_EN")));	
	}
}


OscillatorMixer::~OscillatorMixer()
{
}

template<typename T>
void OscillatorMixer::__RenderBlock(AudioBuffer<T>& buffer)
{
	for (auto&& elem : __oscillators)
	{
		if (*(std::get<2>(elem))) {
			AudioBuffer<T> tmp(buffer.getNumChannels(), buffer.getNumSamples());
			std::get<0>(elem).RenderBlock(tmp);
			for (size_t i = 0; i < tmp.getNumChannels(); i++)
			{
				buffer.addFrom(i, 0, tmp, i, 0, tmp.getNumSamples(), *(std::get<1>(elem)));
			}
		}
	}
}


void OscillatorMixer::AddNoteCommand(int offset, int note, uint8 vel, bool isOn) {
	for (auto&& elem : __oscillators)
	{
		if (*(std::get<2>(elem))) {
			std::get<0>(elem).AddNoteCommand(offset, note, vel, isOn);
		}
	}
}

void OscillatorMixer::ProccesNoteCommand(int note, uint8 vel, bool isOn)
{
	//We don't want to call this here, the oscillators will cal this themself.
}

void OscillatorMixer::ProccessCommand(MidiMessage message)
{
	for (auto&& elem : __oscillators)
	{
		if (*(std::get<2>(elem))) {
			std::get<0>(elem).ProccessCommand(message);
		}
	}
}


void OscillatorMixer::RegisterParameters(int ID)
{
	for (size_t i = 0; i < 4; i++)
	{
		Global->paramHandler->RegisterBool(i, "OSC_MIX_EN", "Enable Oscillator", false);
		Global->paramHandler->RegisterFloat(i, "OSC_MIX_AMP", "Oscillator Amplitude", 0.0f, 1.0f, 0.5f);
	}


}
