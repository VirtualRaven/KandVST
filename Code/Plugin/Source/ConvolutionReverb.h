#ifndef CONVOLUTION_REVERB_H
#define CONVOLUTION_REVERB_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"

template<typename T>
class ConvolutionReverb :
	public IEffect<T>,
	public IVSTParameters
{

private:
	juce::dsp::Convolution __conv;
	
public:
	ConvolutionReverb(int ID,double sampleRate);
	~ConvolutionReverb();
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	static void RegisterParameters(int ID);
	

	virtual void ProccessCommand(MidiMessage message) override;
};


#endif
