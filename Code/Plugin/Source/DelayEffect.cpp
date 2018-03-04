#include "DelayEffect.h"
#define FADE 0.3


DelayEffect::DelayEffect(int ID,double sampleRate):
	IEffect(sampleRate),
	IVSTParameters(ID),
	__delayBuffer(2,static_cast<int>(sampleRate/4)), //1/6 sec echo 
	__delayPos(0)
{
	__delayMultiplier = Global->paramHandler->Get<AudioParameterFloat>(ID, "EX_DELAYMULTI");

	for (int i = 0; i < __delayBuffer.getNumSamples(); i++)
	{
		__delayBuffer.setSample(0, i,0);
		__delayBuffer.setSample(1, i, 0);

	}
}


DelayEffect::~DelayEffect()
{
}

void DelayEffect::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYMULTI", "Delay", 0.0f, 1.0f, 0.2f);
}

template<typename T>
bool DelayEffect::__RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	// TODO: handle empty
	
	double count = 0;
	for (int i = 0; i < len; i++)
	{
		if (__delayBuffer.getSample(0, i) == 0 & __delayBuffer.getSample(1, i) ==  0)
			count++;
	}
	if (count != len) {
		for (int i = 0; i < len; i++)
		{
				
			float multi = *__delayMultiplier;

			__delayBuffer.setSample(0, __delayPos, buffer.getSample(0, i) + __delayBuffer.getSample(0, __delayPos)*multi);
			__delayBuffer.setSample(1, __delayPos, buffer.getSample(1, i) + __delayBuffer.getSample(1, __delayPos)*multi);
			__delayPos = (__delayPos + 1) % __delayBuffer.getNumSamples();
			buffer.addSample(0, i, static_cast<T>(__delayBuffer.getSample(0, __delayPos))*multi);
			buffer.addSample(1, i, static_cast<T>(__delayBuffer.getSample(1, __delayPos))*multi);

		}
	}
	return true;
}

template bool DelayEffect::__RenderBlock(AudioBuffer<double>& buffer, int len, bool empty);
template bool DelayEffect::__RenderBlock(AudioBuffer<float>& buffer, int len, bool empty);

void DelayEffect::ProccessCommand(MidiMessage message)
{
}
