#include "ExampleEffect.h"
#define FADE 0.3


ExampleEffect::ExampleEffect(double sampleRate):
	IEffect(sampleRate),
	__delayBuffer(2,sampleRate/4), //1/6 sec echo 
	__delayPos(0)
{
	for (size_t i = 0; i < __delayBuffer.getNumSamples(); i++)
	{
		__delayBuffer.setSample(0, i,0);
		__delayBuffer.setSample(1, i, 0);

	}
}


ExampleEffect::~ExampleEffect()
{
}

template<typename T>
void ExampleEffect::__RenderBlock(AudioBuffer<T>& buffer)
{
	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		
		__delayBuffer.setSample(0, __delayPos, buffer.getSample(0, i)+__delayBuffer.getSample(0,__delayPos)*FADE);
		__delayBuffer.setSample(1, __delayPos, buffer.getSample(1, i)+__delayBuffer.getSample(0, __delayPos)*FADE);
		__delayPos = (__delayPos + 1) % __delayBuffer.getNumSamples();
		buffer.addSample(0, i, __delayBuffer.getSample(0, __delayPos)*FADE);
		buffer.addSample(1, i, __delayBuffer.getSample(1, __delayPos)*FADE);

	}
}

template void ExampleEffect::__RenderBlock(AudioBuffer<double>& buffer);
template void ExampleEffect::__RenderBlock(AudioBuffer<float>& buffer);

void ExampleEffect::ProccessCommand(MidiMessage message)
{
}
