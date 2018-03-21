#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"
#define FADE 0.3

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__conv(),
	__maxBuffHint(maxBuffHint),
	__responseBuffer()
{


	// Read response
	File resp = File(File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/resp.wav"));

	AudioFormatManager manager;
	manager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = manager.createReaderFor(resp);

	__responseBuffer.setSize(2, reader->lengthInSamples, false, false, false);
	reader->read(&__responseBuffer, 0, reader->lengthInSamples, 0, true, true);

	__responseBufferLen = reader->lengthInSamples;
}

template<typename T>
ConvolutionReverb<T>::~ConvolutionReverb()
{
}
template<typename T>
void ConvolutionReverb<T>::RegisterParameters(int ID)
{
}

template<typename T>
bool ConvolutionReverb<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	// Divide impulse response
	ScopedPointer<dsp::FFT> fft = new dsp::FFT(len);

	AudioSampleBuffer output = AudioSampleBuffer(1, len * 2);
	output.clear();

	for (int i = 0; i < __responseBufferLen / len; i++)
	{
		AudioSampleBuffer h = AudioSampleBuffer(1, len * 2);
		h.copyFrom(0, 0, __responseBuffer, 0, i * len, len);

		fft->performRealOnlyForwardTransform(h.getArrayOfWritePointers()[0], true);

		AudioSampleBuffer in = AudioSampleBuffer();
		in.makeCopyOf(buffer, false);
		in.setSize(1, len * 2, true, true, false);

		fft->performRealOnlyForwardTransform(in.getArrayOfWritePointers()[0]);

		FloatVectorOperations::addWithMultiply(output.getArrayOfWritePointers()[0], in.getArrayOfWritePointers()[0], h.getArrayOfWritePointers()[0], len);
	}

	fft->performRealOnlyInverseTransform(output.getArrayOfWritePointers()[0]);

	for (int i = 0; i < len; i++)
	{
		buffer.setSample(0, i, output.getSample(0, i));
		buffer.setSample(1, i, output.getSample(0, i));
	}
	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

