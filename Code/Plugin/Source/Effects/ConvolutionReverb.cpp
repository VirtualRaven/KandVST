#include "ConvolutionReverb.h"
#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"
#include "Resources_files.h"

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint, GLOBAL *global) :
	__global(global),
	IEffect(sampleRate),
	__sampleRate(sampleRate),
	IVSTParameters(ID),
	__maxBlockSize(maxBuffHint),
	__responseBuffer(),
	__maxBuffHint(maxBuffHint),
	__prevIsEnabled(true),
	__irFromFile(false),
	__prevIrName(""),
	__formatManager(),
	__prevIsEmpty(false),
	__emptyCounter(0)
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
	// Possible error message
	String info = String();
	if (!file.exists())
	{
		info = "File " + String(file.getFileName()) + " does not exist.";
	}
	else
	{
		info = "File: " + String(file.getFileName()) + ", Size: " + String(file.getSize()) + " bytes";
	}
		
	__loadImpulseResponse(__formatManager.createReaderFor(file), info);
}

template<typename T>
void ConvolutionReverb<T>::LoadInputResponse(String irName)
{
	//StringArray ir = StringArray("Nuclear reactor", "Cathedral", "Living room 1", "Living room 2", "Empty room", "Bathtub");

	const void *data;
	size_t length = 0;

	if (irName == "Living room 1")
	{
		data = Resources::IR::living_room1_wav;
		length = sizeof(Resources::IR::living_room1_wav);
	}
	else if (irName == "Living room 2")
	{
		data = Resources::IR::living_room2_wav;
		length = sizeof(Resources::IR::living_room2_wav);
	}
	else if (irName == "Bathtub")
	{
		data = Resources::IR::bathtub_wav;
		length = sizeof(Resources::IR::bathtub_wav);
	}
	else if (irName == "Nuclear reactor")
	{
		data = Resources::IR::r1_nuclear_reactor_cut_wav;
		length = sizeof(Resources::IR::r1_nuclear_reactor_cut_wav);
	}
	else if (irName == "Cathedral")
	{
		data = Resources::IR::cathedral_minster_york_wav;
		length = sizeof(Resources::IR::cathedral_minster_york_wav);
	}
	else if (irName == "Empty room")
	{
		data = Resources::IR::empty_apartment_bedroom_wav;
		length = sizeof(Resources::IR::empty_apartment_bedroom_wav);
	}
	else
	{
		return;
	}

	MemoryInputStream *mem; // Will be deleted automatically
	mem = new MemoryInputStream(data, length, false);

	String info = "Internal name: " + irName + ", " + "Size: " + String(length) + " bytes";
	__loadImpulseResponse(__formatManager.createReaderFor(mem), info);
}

template<typename T>
void ConvolutionReverb<T>::__loadImpulseResponse(ScopedPointer<AudioFormatReader> reader, String errorInfo = "")
{
	__prevInputs.clear();
	__inputBlocks.clear();

	// Check if reader is null
	if (reader == nullptr)
	{
		NativeMessageBox::showMessageBoxAsync(
			AlertWindow::AlertIconType::WarningIcon, 
			"Reverb Error", 
			"Could not load selected impulse reponse.\n" + errorInfo
		);

		__responseBlocks.clear();
		__responseBuffer.clear();

		// Reverb will be disabled until the user selects a different IR
		return;
	}

	// Read the ir
	AudioSampleBuffer readBuff;
	readBuff.setSize(2, reader->lengthInSamples, false, false, false);
	readBuff.clear();
	reader->read(&readBuff, 0, reader->lengthInSamples, 0, true, true);

	double sampleRateRatio = reader->sampleRate / __sampleRate;

	if (sampleRateRatio != 1.0)
	{
		// Resample
		LagrangeInterpolator interpLeft;
		LagrangeInterpolator interpRight;

		int newLen = roundDoubleToInt(double(reader->lengthInSamples) / sampleRateRatio);
		__responseBufferLen = nextPowerOfTwo(newLen);
		__responseBuffer.setSize(2, __responseBufferLen, false, false, false);
		__responseBuffer.clear();
		
		interpLeft.process(sampleRateRatio, readBuff.getWritePointer(0), __responseBuffer.getWritePointer(0), newLen);
		interpRight.process(sampleRateRatio, readBuff.getWritePointer(1), __responseBuffer.getWritePointer(1), newLen);
	}
	else
	{
		// Just copy
		__responseBufferLen = nextPowerOfTwo(reader->lengthInSamples);
		__responseBuffer.setSize(2, __responseBufferLen, false, false, false);
		__responseBuffer.clear();
		__responseBuffer.copyFrom(0, 0, readBuff, 0, 0, reader->lengthInSamples);
		__responseBuffer.copyFrom(1, 0, readBuff, 1, 0, reader->lengthInSamples);
	}

	//__responseBufferLen /= 2;

	__createResponseBlocks(__maxBlockSize);
}

template<typename T>
int ConvolutionReverb<T>::__getAudioBufferListSampleCount(std::list<AudioSampleBuffer> list)
{
	int ret = 0;

	for (auto buffer : list)
	{
		ret += buffer.getNumSamples();
	}

	return ret;
}



template<typename T>
void ConvolutionReverb<T>::__createResponseBlocks(int len)
{
	__responseBlocks.clear();
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
	StringArray ir = StringArray("Nuclear reactor", "Cathedral", "Living room 1", "Living room 2", "Empty room", "Bathtub");
	global->paramHandler->RegisterChoice(ID, "REVERB_IR", "TYPE", ir, 0);
}

template<typename T>
bool ConvolutionReverb<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	// Check if ir has changed (even if disabled)
	auto currentIrName = __ir->getCurrentChoiceName();
	if (!__irFromFile && currentIrName.compare(__prevIrName) != 0)
	{
		LoadInputResponse(currentIrName);

		__prevIrName = currentIrName;
		return false;
	}

	//Check if enabled
	if ((*__isEnabled == false && __prevIsEnabled))
	{
		// Clean everything
		__prevInputs.clear();
		__inputBlocks.clear();

		// Cancel counter
		__emptyCounter = 0;

		__prevIsEnabled = *__isEnabled;
		return false;
	}

	if (*__isEnabled == false)
		return false;

	// If empty == true for the first time, let the reverb run to the end
	if (empty && !__prevIsEmpty)
	{
		// Start counter
		__emptyCounter = __responseBlocks.size();
	}
	else if (!empty && __prevIsEmpty)
	{
		// Cancel counter
		__emptyCounter = 0;
	}
	else if (empty && __prevIsEmpty)
	{
		if (__emptyCounter > 0)
			__emptyCounter--;

		// The reverb is completely off
		if (__emptyCounter == 0)
			return false;
	}

	__prevIsEmpty = empty;

	if (__responseBlocks.size() == 0)
		return false;

	auto bufferp = buffer.getArrayOfWritePointers();

	// If len has changed
	/*if (__prevBlockSize != len)
	{
		// This occurs when looping in a DAW
		// Process as if len hasn't been changed
		//__global->log->Write("Blocksize: " + std::to_string(inLen) + "\n");
	}*/
	__global->log->Write("Blocksize: " + std::to_string(len) + "\n");

	// Block size needs to be a power of two
	int blockSize = nextPowerOfTwo(__maxBlockSize);

	// x: Input buffer 2*blockSize
	AudioSampleBuffer x = AudioSampleBuffer(2, 4*blockSize);
	x.clear();
	auto xp = x.getArrayOfWritePointers();

	// in: The actual current input of length len
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
	if (__getAudioBufferListSampleCount(__prevInputs) > roundDoubleToInt((2 * blockSize)))
		__prevInputs.pop_back();

	// Make x contain the current input to the right and previous inputs to the left of it
	int prevLeft = 0;
	for (auto prev : __prevInputs)
	{
		auto prevp = prev.getArrayOfReadPointers();
		prevLeft += prev.getNumSamples();

		for (int j = 0; j < prev.getNumSamples(); j++)
		{
			if ((2 * blockSize - prevLeft + j) < 0)
				continue;

			xp[0][2 * blockSize - prevLeft + j] = prevp[0][j];
			xp[1][2 * blockSize - prevLeft + j] = prevp[1][j];
		}
	}

	__fft->performRealOnlyForwardTransform(x.getWritePointer(0));
	__fft->performRealOnlyForwardTransform(x.getWritePointer(1));

	// Put the new TRANSFORMED input block in front of the list, remove the last if nessesary
	__inputBlocks.push_front(x);
	if (__inputBlocks.size() > __responseBlocks.size())
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

		float outLeft = (wetOutLeft*wet + dryOutLeft*dry);
		float outRight = (wetOutRight*wet + dryOutRight*dry);

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
	// Clear all previous values
	//__inputBlocks.clear();
	//__prevInputs.clear();
	//__emptyCounter = 0;
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

