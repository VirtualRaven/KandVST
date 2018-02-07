#ifndef EXAMPLE_EFFECT_H
#define EXAMPLE_EFFECT_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"

class ExampleEffect :
	public IEffect
{

private:
	template<typename T>
	void __RenderBlock(AudioBuffer<T>& buffer);
	AudioBuffer<double> __delayBuffer;
	int __delayPos;

	AudioParameterFloat* __delayMultiplier;
	
public:
	ExampleEffect(double sampleRate);
	~ExampleEffect();

	// Inherited via IEffect
	virtual void RenderBlock(AudioBuffer<float>& buffer) override 
	{
		__RenderBlock(buffer);
	}
	virtual void RenderBlock(AudioBuffer<double>& buffer) override
	{
		__RenderBlock(buffer);
	}
	virtual void ProccessCommand(MidiMessage message) override;
};


#endif //EXAMPLE_EFFECT_H
