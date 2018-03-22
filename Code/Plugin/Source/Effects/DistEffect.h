#ifndef DIST_H
#define DIST_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"

template<typename T>
class DistEffect :
	public IEffect<T>
{
private:
	AudioParameterFloat* __threshold;
	AudioParameterInt* __lfoIndex;
	AudioParameterBool* __isActive;
public:
	DistEffect(int ID, double samplerate);
	~DistEffect();
	static void RegisterParameters(int ID);
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	virtual void ProccessCommand(MidiMessage message) override;
};
#endif