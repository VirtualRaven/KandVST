#include "ExampleEffect.h"
#define FADE 0.3


ExampleEffect::ExampleEffect(int ID,double sampleRate):
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


ExampleEffect::~ExampleEffect()
{
}

void ExampleEffect::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYMULTI", "Delay", 0.0f, 1.0f, 0.2f);
}

template<typename T>
void ExampleEffect::__RenderBlock(AudioBuffer<T>& buffer)
{
	for (int i = 0; i < buffer.getNumSamples(); i++)
	{

		float multi = *__delayMultiplier;
		
		__delayBuffer.setSample(0, __delayPos, buffer.getSample(0, i)+__delayBuffer.getSample(0,__delayPos)*multi);
		__delayBuffer.setSample(1, __delayPos, buffer.getSample(1, i)+__delayBuffer.getSample(0, __delayPos)*multi);
		__delayPos = (__delayPos + 1) % __delayBuffer.getNumSamples();
		buffer.addSample(0, i, __delayBuffer.getSample(0, __delayPos)*multi);
		buffer.addSample(1, i, __delayBuffer.getSample(1, __delayPos)*multi);

	}
}

template void ExampleEffect::__RenderBlock(AudioBuffer<double>& buffer);
template void ExampleEffect::__RenderBlock(AudioBuffer<float>& buffer);

void ExampleEffect::ProccessCommand(MidiMessage message)
{
}
