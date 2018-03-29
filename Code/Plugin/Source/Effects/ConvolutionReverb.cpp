#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"
#include "Resources_files.h"

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint, GLOBAL *global) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__prevBlockSize(maxBuffHint),
	__responseBuffer(),
	__overlapBufferLen(0),
	__maxBuffHint(maxBuffHint),
	__prevIsEnabled(true),
	__irFromFile(false),
	__prevIrName(""),
	__formatManager()
{
	__formatManager.registerBasicFormats();

	__isEnabled = global->paramHandler->Get<AudioParameterBool>(ID, "REVERB_EN");
	__dryGain = global->paramHandler->Get<AudioParameterFloat>(ID, "REVERB_DRY");
	__wetGain = global->paramHandler->Get<AudioParameterFloat>(ID, "REVERB_WET");
	__ir = global->paramHandler->Get<AudioParameterChoice>(ID, "REVERB_IR");
}

template<typename T>
void ConvolutionReverb<T>::LoadInputResponse(File file)
{
	__loadImpulseResponse(__formatManager.createReaderFor(file));
}

template<typename T>
void ConvolutionReverb<T>::LoadInputResponse(String irName)
{
	MemoryInputStream *mem; // Will be deleted automatically

	if (irName == "Living room 1")
	{
		mem = new MemoryInputStream(Resources::IR::living_room1_wav, sizeof(Resources::IR::living_room1_wav), false);
	}
	else if (irName == "Living room 2")
	{
		mem = new MemoryInputStream(Resources::IR::living_room2_wav, sizeof(Resources::IR::living_room2_wav), false);
	}
	else if (irName == "Bathtub")
	{
		mem = new MemoryInputStream(Resources::IR::bathtub_wav, sizeof(Resources::IR::bathtub_wav), false);
	}
	else
	{
		return;
	}

	__loadImpulseResponse(__formatManager.createReaderFor(mem));
}

template<typename T>
void ConvolutionReverb<T>::__loadImpulseResponse(ScopedPointer<AudioFormatReader> reader)
{
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
void ConvolutionReverb<T>::RegisterParameters(int ID, GLOBAL *global)
{
	global->paramHandler->RegisterBool(ID, "REVERB_EN", "REVERB", 0);
	global->paramHandler->RegisterFloat(ID, "REVERB_DRY", "DRY", 0.0, 1.0, 1.0);
	global->paramHandler->RegisterFloat(ID, "REVERB_WET", "WET", 0.0, 1.0, 0.6);
	StringArray ir = StringArray("Living room 1", "Living room 2", "Bathtub");
	global->paramHandler->RegisterChoice(ID, "REVERB_IR", "TYPE", ir, 0);
}

template<typename T>
bool ConvolutionReverb<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	//Check if enabled
	if (*__isEnabled == false && __prevIsEnabled)
	{
		// Clean everything
		__prevInput.clear();
		__prevInputs.clear();

		__prevIsEnabled = *__isEnabled;
		return false;
	}

	if (*__isEnabled == false)
		return false;

	if (!__irFromFile && __ir->getCurrentChoiceName().compare(__prevIrName) != 0)
	{
		// The ir choice has changed
		LoadInputResponse(__ir->getCurrentChoiceName());

		__prevIrName = __ir->getCurrentChoiceName();
		return false;
	}

	if (__responseBlocks.size() == 0)
		return false;

	// Block size needs to be a power of two
	int blockSize = nextPowerOfTwo(len);

	auto bufferp = buffer.getArrayOfWritePointers();

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
	auto xp = x.getArrayOfWritePointers();

	// in: The actual current input of length len (does not need to be cleaned)
	AudioSampleBuffer in = AudioSampleBuffer(2, len);
	auto inp = in.getArrayOfWritePointers();

	for (int i = 0; i < len; i++)
	{
		// double -> float
		inp[0][i] = static_cast<float>(bufferp[0][i]);
		inp[1][i] = static_cast<float>(bufferp[1][i]);
	}

	// Put the new input block in front of the list, remove the last if nessesary
	__prevInputs.push_front(in);
	if (__prevInputs.size() > roundDoubleToInt((2 * blockSize) / len))
		__prevInputs.pop_back();

	// Make x contain the current input to the right and previous inputs to the left of it
	int i = 1;
	for (auto prev : __prevInputs)
	{
		auto prevp = prev.getArrayOfReadPointers();

		for (int j = 0; j < len; j++)
		{
			if (i*len > 2 * blockSize)
				continue;

			xp[0][(2 * blockSize - i * len) + j] = prevp[0][j];
			xp[1][(2 * blockSize - i * len) + j] = prevp[1][j];
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
	auto convOutputp = convOutput.getArrayOfWritePointers();

	int b = 0;
	for (auto currentIn : __inputBlocks)
	{
		// Complex multiplication (stereo)
		for (int channel = 0; channel < 2; channel++)
		{
			float *y = convOutputp[channel];
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

	// Output dry/wet mix
	float wet = *__wetGain;
	float dry = *__dryGain;

	for (int i = 0; i < len; i++)
	{
		// Last len samples are the wet output (float -> double)
		float wetOutLeft = convOutputp[0][i + 2 * blockSize - len];
		float wetOutRight = convOutputp[1][i + 2 * blockSize - len];

		float dryOutLeft = bufferp[0][i];
		float dryOutRight = bufferp[1][i];

		float outLeft = (wetOutLeft*wet + dryOutLeft*dry) / 2;
		float outRight = (wetOutRight*wet + dryOutRight*dry) / 2;

		bufferp[0][i] = outLeft;
	    bufferp[1][i] = outRight;
	}

	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template<typename T>
void ConvolutionReverb<T>::Reset()
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

