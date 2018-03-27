#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__prevBlockSize(maxBuffHint),
	__responseBuffer(),
	__overlapBufferLen(0),
	__maxBuffHint(maxBuffHint)
{
}

template<typename T>
void ConvolutionReverb<T>::LoadInputResponse(File file)
{
	AudioFormatManager manager;
	manager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = manager.createReaderFor(file);

	__responseBufferLen = nextPowerOfTwo(reader->lengthInSamples);
	__responseBuffer.setSize(2, __responseBufferLen, false, false, false);
	__responseBuffer.clear();
	reader->read(&__responseBuffer, 0, reader->lengthInSamples, 0, true, true);

	__responseBufferLen /= 2;

	__createResponseBlocks(__maxBuffHint);
}

template<typename T>
void ConvolutionReverb<T>::__createResponseBlocks(int len)
{
	int blockSize = nextPowerOfTwo(len);

	__fft = new dsp::FFT(roundDoubleToInt(log2(2 * blockSize)));
	__ifft = new dsp::FFT(roundDoubleToInt(log2(2 * blockSize)));

	// Create impulse response blocks
	for (int i = 0; i < roundDoubleToInt((__responseBufferLen / len)); i++)
	{
		AudioSampleBuffer nextBlock = AudioSampleBuffer(2, 4 * blockSize);
		nextBlock.clear();

		nextBlock.copyFrom(0, 0, __responseBuffer, 0, i*len, len);
		nextBlock.copyFrom(1, 0, __responseBuffer, 1, i*len, len);
		__fft->performRealOnlyForwardTransform(nextBlock.getWritePointer(0));
		__fft->performRealOnlyForwardTransform(nextBlock.getWritePointer(1));

		__responseBlocks.push_back(nextBlock);
	}
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
	// Block size needs to be a power of two
	int blockSize = nextPowerOfTwo(len);

	// If len has changed
	if (__prevBlockSize != len)
	{
		__prevBlockSize = len;

		__prevInput.clear();
		__prevInputs.clear();
		__responseBlocks.clear();

		__fft = new dsp::FFT(roundDoubleToInt(log2(2*blockSize)));
		__ifft = new dsp::FFT(roundDoubleToInt(log2(2*blockSize)));

		__createResponseBlocks(len);
	}

	// x: Input of length 2*blockSize
	AudioSampleBuffer x = AudioSampleBuffer(2, 4*blockSize);
	x.clear();

	// in: The actual current input of length len (does not need to be cleaned)
	AudioSampleBuffer in = AudioSampleBuffer(2, len);

	for (int i = 0; i < len; i++)
	{
		// double -> float
		in.setSample(0, i, buffer.getSample(0, i));
		in.setSample(1, i, buffer.getSample(1, i));
	}

	// Put the new input block in front of the list, remove the last if nessesary
	__prevInputs.push_front(in);
	if (__prevInputs.size() > roundDoubleToInt((2 * blockSize) / len))
		__prevInputs.pop_back();

	// Make x contain the current input to the right and previous inputs to the left of it
	int i = 1;
	for (auto prev : __prevInputs)
	{
		for (int j = 0; j < len; j++)
		{
			if (i*len > 2 * blockSize)
				continue;

			x.setSample(0, (2 * blockSize - i * len) + j, prev.getSample(0, j));
			x.setSample(1, (2 * blockSize - i * len) + j, prev.getSample(1, j));
		}

		i++;
	}

	__fft->performRealOnlyForwardTransform(x.getWritePointer(0));
	__fft->performRealOnlyForwardTransform(x.getWritePointer(1));

	// Put the new TRANSFORMED input block in front of the list, remove the last if nessesary
	__inputBlocks.push_front(x);
	if (__inputBlocks.size() >= __responseBlocks.size())
		__inputBlocks.pop_back();


	// Multiply to output
	AudioSampleBuffer convOutput = AudioSampleBuffer(2, 4*blockSize);
	convOutput.clear();

	int b = 0;
	for (auto currentIn : __inputBlocks)
	{
		// Complex multiplication (stereo)
		for (int channel = 0; channel < 2; channel++)
		{
			float *y = convOutput.getWritePointer(channel);
			const float *in = currentIn.getWritePointer(channel);
			const float *h = __responseBlocks.at(b).getReadPointer(channel);

			for (int i = 0; i < blockSize; i += 2)
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
		b++;
	}

	// Inverse tranform
	__ifft->performRealOnlyInverseTransform(convOutput.getWritePointer(0));
	__ifft->performRealOnlyInverseTransform(convOutput.getWritePointer(1));

	// The first len samples is the output
	for (int i = 0; i < len; i++)
	{
		// Add overlap (float -> double)
		buffer.setSample(0, i, convOutput.getSample(0, i + 2*blockSize - len));
	    buffer.setSample(1, i, convOutput.getSample(1, i + 2*blockSize - len));
	}

	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

