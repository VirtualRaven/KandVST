#ifndef CONVOLUTION_REVERB_H
#define CONVOLUTION_REVERB_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"
#include <list>
#include "juce_dsp/juce_dsp.h"
#include "Resources_files.h"

template<typename T>
class ConvolutionReverb :
	public IEffect<T>,
	public IVSTParameters
{

private:
	GLOBAL *__global;
	double __sampleRate;

	AudioFormatManager __formatManager;

	AudioSampleBuffer __responseBuffer;

	AudioSampleBuffer __overlap;

	ScopedPointer<dsp::FFT> __fft;
	ScopedPointer<dsp::FFT> __ifft;

	std::vector<AudioSampleBuffer> __responseBlocks;
	std::list<AudioSampleBuffer> __inputBlocks;
	std::list<AudioSampleBuffer> __prevInputs;
	
	int __responseBufferLen;
	int __maxBlockSize;
	int __inputPosInBlock;

	AudioParameterBool *__isEnabled;
	AudioParameterFloat *__dryGain;
	AudioParameterFloat *__wetGain;
	AudioParameterChoice *__ir;

	bool __prevIsEnabled;
	bool __prevIsEmpty;
	int __emptyCounter;
	bool __irFromFile;
	String __prevIrName;

	void __createResponseBlocks(int len);
	void __loadImpulseResponse(ScopedPointer<AudioFormatReader> reader, String errorInfo);
	int __getAudioBufferListSampleCount(std::list<AudioSampleBuffer> list);

public:
	ConvolutionReverb(int ID,double sampleRate, int maxBuffHint, GLOBAL *global);
	void LoadInputResponse(File file);
	void LoadInputResponse(String irName);
	~ConvolutionReverb();
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	static void RegisterParameters(int ID, GLOBAL *global);

	virtual void ProccessCommand(MidiMessage message) override;
	virtual void Reset() override;
};


#endif
