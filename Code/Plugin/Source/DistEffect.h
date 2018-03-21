#ifndef DIST_H
#define DIST_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"

template<typename T>
class DistEffect :
	public IEffect<T>
{
private:
	AudioParameterFloat* __threshold;
public:
	DistEffect(int ID, double samplerate);
	~DistEffect();
	static void RegisterParameters(int ID);
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	virtual void ProccessCommand(MidiMessage message) override;
};
#endif