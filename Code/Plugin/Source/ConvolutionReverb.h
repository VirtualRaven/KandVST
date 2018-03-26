#ifndef CONVOLUTION_REVERB_H
#define CONVOLUTION_REVERB_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"
#include "juce_dsp/juce_dsp.h"

template<typename T>
class ConvolutionReverb :
	public IEffect<T>,
	public IVSTParameters
{

private:
	int __maxBuffHint;

	AudioSampleBuffer __responseBuffer;
	AudioSampleBuffer __overlapBuffer;
	AudioSampleBuffer __resTransform;
	ScopedPointer<dsp::FFT> __fft;
	ScopedPointer<dsp::FFT> __ifft;
	std::vector<AudioSampleBuffer> __responseBlocks;
	int __overlapBufferLen;
	int __responseBufferLen;
	int __prevBlockSize;

	void fft(float *samples, int len);
	void ifft(float *samples, int len);

public:
	ConvolutionReverb(int ID,double sampleRate, int maxBuffHint);
	~ConvolutionReverb();
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	static void RegisterParameters(int ID);
	

	virtual void ProccessCommand(MidiMessage message) override;
};


#endif
