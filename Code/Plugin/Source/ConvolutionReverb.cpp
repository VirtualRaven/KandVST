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
	__responseBlocks.clear();

	// Read response
	File resp = File(File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/resp.wav"));

	AudioFormatManager manager;
	manager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = manager.createReaderFor(resp);

	__responseBufferLen = nextPowerOfTwo(reader->lengthInSamples);
	__responseBuffer.setSize(2, __responseBufferLen, false, false, false);
	__responseBuffer.clear();
	reader->read(&__responseBuffer, 0, reader->lengthInSamples, 0, true, true);
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
void ConvolutionReverb<T>::fft(float *samples, int len)
{
	__fft->performRealOnlyForwardTransform(samples, false);
	// Reorganize?
}

template<typename T>
void ConvolutionReverb<T>::ifft(float *samples, int len)
{
}

template<typename T>
bool ConvolutionReverb<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	int blockSize = nextPowerOfTwo(len);

	/*/ Set overlapBuffer size
	if (__overlapBufferLen == 0)
	{
		__overlapBuffer.setSize(1, fftSize, false, false, true);
		__overlapBuffer.clear();
		__overlapBufferLen = fftSize;
	}*/

	// Create impulse transform
	if (__prevBlockSize != len)
	{
		__fft = new dsp::FFT(roundDoubleToInt(log2(2*blockSize)));
		__ifft = new dsp::FFT(roundDoubleToInt(log2(2*blockSize)));

		// Create impulse response blocks
		for (int i = 0; i < roundDoubleToInt((__responseBufferLen / blockSize)); i++)
		{
			AudioSampleBuffer nextBlock = AudioSampleBuffer(1, 4 * blockSize);
			nextBlock.clear();
			nextBlock.copyFrom(0, 0, __responseBuffer, 0, i*blockSize, blockSize);
			__fft->performRealOnlyForwardTransform(nextBlock.getWritePointer(0));
			__responseBlocks.push_back(nextBlock);
		}

		__prevBlockSize = len;
	}

	// x: input padded with 0
	AudioSampleBuffer x = AudioSampleBuffer(1, 2*blockSize);
	x.clear();
	for (int i = 0; i < len; i++)
	{
		// double -> float
		x.setSample(0, i, buffer.getSample(0, i));
	}

	__fft->performRealOnlyForwardTransform(x.getWritePointer(0), false);

	// Multiply to output
	AudioSampleBuffer convOutput = AudioSampleBuffer(1, 4*blockSize);
	convOutput.clear();

	for (int b = 0; b < __responseBlocks.size(); b++)
	{
		//FloatVectorOperations::addWithMultiply(convOutput.getWritePointer(0), x.getWritePointer(0), __responseBlocks.at(b).getWritePointer(0), blockSize);

		// Complex multiplication
		float *y = convOutput.getWritePointer(0);
		const float *in = x.getReadPointer(0);
		const float *h = __responseBlocks.at(b).getReadPointer(0);

		for (int i = 0; i < blockSize; i+=2)
		{
			// (a + bi)(c + di) = (ac - bd) + (bc + ad)i
			float xr = in[i];
			float xi = in[i + 1];
			float hr = h[i];
			float hi = h[i + 1];

			// Real
			y[i] += xr * hr - xi * hi;

			// Imaginary
			y[i + 1] += xi * hr + xr * hi;
		}
	}

	// Inverse tranform
	__ifft->performRealOnlyInverseTransform(convOutput.getWritePointer(0));

	// The first len samples is the output
	for (int i = 0; i < len; i++)
	{
		// Add overlap (float -> double)
		buffer.setSample(0, i, convOutput.getSample(0, i));
		buffer.setSample(1, i, convOutput.getSample(0, i));
	}

	/* The rest needs to be added to the overlapBuffer
	AudioSampleBuffer shiftedOverlap = AudioSampleBuffer(1, __overlapBufferLen);
	shiftedOverlap.clear();
	shiftedOverlap.copyFrom(0, 0, __overlapBuffer, 0, len, __overlapBufferLen - len);

	AudioSampleBuffer newOverlap = AudioSampleBuffer(1, __overlapBufferLen);
	newOverlap.clear();
	newOverlap.copyFrom(0, 0, convOutput, 0, len, __overlapBufferLen - len);

	// New overlap
	FloatVectorOperations::add(__overlapBuffer.getWritePointer(0), shiftedOverlap.getWritePointer(0), newOverlap.getWritePointer(0), __overlapBufferLen);
	*/

	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

