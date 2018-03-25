#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__maxBuffHint(maxBuffHint),
	__overlapBuffer(),
	__responseBuffer(),
	__resTransform(),
	__overlapBufferLen(0),
	__prevBlockSize(0)
{


	// Read response
	File resp = File(File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/resp.wav"));

	AudioFormatManager manager;
	manager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = manager.createReaderFor(resp);

	__responseBuffer.setSize(2, reader->lengthInSamples, false, false, false);
	__responseBuffer.clear();
	reader->read(&__responseBuffer, 0, reader->lengthInSamples, 0, true, true);

	__responseBufferLen = reader->lengthInSamples/10;
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
	int fftSize = nextPowerOfTwo(len + __responseBufferLen - 1);

	// Set overlapBuffer size
	if (__overlapBufferLen == 0)
	{
		__overlapBuffer.setSize(1, fftSize, false, false, true);
		__overlapBuffer.clear();
		__overlapBufferLen = fftSize;
	}

	// Create impulse transform
	if (__prevBlockSize != len)
	{
		__fft = new dsp::FFT(roundDoubleToInt(log2(fftSize)));

		__resTransform = AudioSampleBuffer(1, 2*fftSize);
		__resTransform.clear();
		__resTransform.copyFrom(0, 0, __responseBuffer, 0, 0, __responseBufferLen);
		__fft->performRealOnlyForwardTransform(__resTransform.getWritePointer(0));

		__prevBlockSize = len;
	}

	// x: input padded with 0
	AudioSampleBuffer x = AudioSampleBuffer(1, 2*fftSize);
	x.clear();
	for (int i = 0; i < len; i++)
	{
		// double -> float
		x.setSample(0, i, buffer.getSample(0, i));
	}

	__fft->performRealOnlyForwardTransform(x.getWritePointer(0));

	// Multiply to output
	AudioSampleBuffer convOutput = AudioSampleBuffer(1, 2*fftSize);
	convOutput.clear();
	FloatVectorOperations::addWithMultiply(convOutput.getWritePointer(0), x.getWritePointer(0), __resTransform.getWritePointer(0), 2 * fftSize);

	// Inverse tranform
	__fft->performRealOnlyInverseTransform(convOutput.getWritePointer(0));

	// The first len samples is the output
	for (int i = 0; i < len; i++)
	{
		// Add overlap (float -> double)
		buffer.setSample(0, i, convOutput.getSample(0, i) + __overlapBuffer.getSample(0, i));
		buffer.setSample(1, i, convOutput.getSample(0, i) + __overlapBuffer.getSample(0, i));
	}

	// The rest needs to be added to the overlapBuffer
	AudioSampleBuffer shiftedOverlap = AudioSampleBuffer(1, __overlapBufferLen);
	shiftedOverlap.clear();
	shiftedOverlap.copyFrom(0, 0, __overlapBuffer, 0, len, __overlapBufferLen - len);

	AudioSampleBuffer newOverlap = AudioSampleBuffer(1, __overlapBufferLen);
	newOverlap.clear();
	newOverlap.copyFrom(0, 0, convOutput, 0, len, __overlapBufferLen - len);

	// New overlap
	FloatVectorOperations::add(__overlapBuffer.getWritePointer(0), shiftedOverlap.getWritePointer(0), newOverlap.getWritePointer(0), __overlapBufferLen);


	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

