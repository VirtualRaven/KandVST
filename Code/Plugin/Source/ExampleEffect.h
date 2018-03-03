#ifndef EXAMPLE_EFFECT_H
#define EXAMPLE_EFFECT_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"
class ExampleEffect :
	public IEffect,
	public IVSTParameters
{

private:
	template<typename T>
	bool __RenderBlock(AudioBuffer<T>& buffer, int len, bool empty);
	AudioBuffer<double> __delayBuffer;
	int __delayPos;

	AudioParameterFloat* __delayMultiplier;
	
public:
	ExampleEffect(int ID,double sampleRate);
	~ExampleEffect();

	static void RegisterParameters(int ID);

	// Inherited via IEffect
	virtual bool RenderBlock(AudioBuffer<float>& buffer, int len, bool empty) override
	{
		return __RenderBlock(buffer, len, empty);
	}
	virtual bool RenderBlock(AudioBuffer<double>& buffer, int len, bool empty) override
	{
		return __RenderBlock(buffer, len, empty);
	}
	virtual void ProccessCommand(MidiMessage message) override;
};


#endif //EXAMPLE_EFFECT_H
