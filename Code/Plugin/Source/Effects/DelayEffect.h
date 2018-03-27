#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"
template<typename T>
class DelayEffect :
	public IEffect<T>,
	public IVSTParameters
{

private:


	AudioBuffer<T> __delayBuffer;
	int __delayLen;
	int __prevDelayLen;
	int __delayPos;
	AudioParameterFloat* __delayLenMult;
	AudioParameterFloat* __delayMultiplier;
	
public:
	DelayEffect(int ID,double sampleRate,GLOBAL*global);
	~DelayEffect();
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	static void RegisterParameters(int ID, GLOBAL*Global);


	virtual void ProccessCommand(MidiMessage message) override;
	virtual void Reset() override;
	GLOBAL * Global;
};


#endif //DELAY_EFFECT_H
