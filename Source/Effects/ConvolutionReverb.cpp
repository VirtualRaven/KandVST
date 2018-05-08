/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ConvolutionReverb.h"
#include "juce_dsp/juce_dsp.h"
#include "Resources_files.h"

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint, GLOBAL *global) :
	IEffect<T>(sampleRate),
	__sampleRate(sampleRate),
	IVSTParameters(ID),
	__prevBlockSize(maxBuffHint),
	__responseBuffer(),
	__maxBuffHint(maxBuffHint),
	__prevIsEnabled(true),
	__irFromFile(false),
	__prevIrName(""),
	__formatManager(),
	__prevIsEmpty(false),
	__emptyCounter(0),
	__blockSizeChangeCounter(0),
	__global(global)
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
	const void *data;
	size_t length = 0;
	//"Staircase 1 (chabo)", "Staircase 2 (chabo)"

	if (irName == "Staircase 1 (chabo)")
	{
		data = Resources::IR::chabo1_wav;
		length = sizeof(Resources::IR::chabo1_wav);
	}
	else if (irName == "Staircase 2 (chabo)")
	{
		data = Resources::IR::chabo2_wav;
		length = sizeof(Resources::IR::chabo2_wav);
	}
	else if (irName == "Church 1 close")
	{
		data = Resources::IR::church1_close_wav;
		length = sizeof(Resources::IR::church1_close_wav);
	}
	else if (irName == "Church 1 far")
	{
		data = Resources::IR::church1_far_wav;
		length = sizeof(Resources::IR::church1_far_wav);
	}
	else if (irName == "Church 2 close")
	{
		data = Resources::IR::church2_close_wav;
		length = sizeof(Resources::IR::church2_close_wav);
	}
	else if (irName == "Church 2 far")
	{
		data = Resources::IR::church2_far_wav;
		length = sizeof(Resources::IR::church2_far_wav);
	}
	else if (irName == "Living room 1")
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
	else if (irName == "Empty room")
	{
		data = Resources::IR::empty_apartment_bedroom_wav;
		length = sizeof(Resources::IR::empty_apartment_bedroom_wav);
	}
	else
	{
		// External IR:
		File extIr = File(String(__getExternalIrDir() + File::getSeparatorString() + irName + ".wav"));
		LoadInputResponse(extIr);

		return;
	}

	MemoryInputStream *mem; // Will be deleted automatically
	mem = new MemoryInputStream(data, length, false);

	String info = "Internal name: " + irName + ", " + "Size: " + String(length) + " bytes";
	__loadImpulseResponse(__formatManager.createReaderFor(mem), info);
}

template<typename T>
void ConvolutionReverb<T>::__loadImpulseResponse(ScopedPointer<AudioFormatReader> reader, String errorInfo )
{
	__prevInputs.clear();
	__inputBlocks.clear();

	// Check if reader is null
	if (reader == nullptr)
	{
		GLOBAL::MessageBoxInfo info;
		info.icon = AlertWindow::AlertIconType::WarningIcon;
		info.title = "Reverb Error";
		info.message = "Could not load selected impulse reponse.\n" + errorInfo;
		__global->ShowMessageBox(info);

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

	__createResponseBlocks(__prevBlockSize);
}

template<typename T>
String ConvolutionReverb<T>::__getExternalIrDir()
{
	return File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/IR");
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

	// Internal IR:
	StringArray ir = StringArray("Church 1 close", "Church 1 far", "Church 2 close", "Church 2 far", 
		"Living room 1", "Living room 2", "Empty room", "Bathtub",
		"Staircase 1 (chabo)", "Staircase 2 (chabo)");

	// External IR:
	File irFolder = File(__getExternalIrDir());

	if (irFolder.exists() == false)
		irFolder.createDirectory();

	for (auto s : irFolder.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav"))
	{
		ir.add(s.getFileNameWithoutExtension());
	}

	global->paramHandler->RegisterChoice(ID, "REVERB_IR", "TYPE", ir, 0);
}

template<typename T>
bool ConvolutionReverb<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	int inLen = len;

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
	if (__prevBlockSize != len)
	{
		// This occurs when looping in a DAW
		// Process as if len hasn't been changed
		inLen = __prevBlockSize;
		__blockSizeChangeCounter++;
	}
	else
	{
		__blockSizeChangeCounter = 0;
	}

	if (__blockSizeChangeCounter == 5)
	{
		NativeMessageBox::showMessageBoxAsync(
			AlertWindow::AlertIconType::WarningIcon,
			"Reverb Error",
			"Your VST host is using a variable buffer size which is not supported by the reverb effect. Please set your host to use a fixed buffer size or disable the reverb effect."
		);

		__blockSizeChangeCounter = 0;
		*__isEnabled = false;
		return false;
	}

	// Block size needs to be a power of two
	int blockSize = nextPowerOfTwo(inLen);

	// x: Input of length 2*blockSize
	AudioSampleBuffer x = AudioSampleBuffer(2, 4*blockSize);
	x.clear();
	auto xp = x.getArrayOfWritePointers();

	// in: The actual current input of length len
	AudioSampleBuffer in = AudioSampleBuffer(2, inLen);
	in.clear();
	auto inp = in.getArrayOfWritePointers();

	for (int i = 0; i < inLen; i++)
	{
		// double -> float
		inp[0][i] = static_cast<float>(bufferp[0][i]);
		inp[1][i] = static_cast<float>(bufferp[1][i]);
	}

	// Put the new input block in front of the list, remove the last if nessesary
	__prevInputs.push_front(in);
	if (__prevInputs.size() > roundDoubleToInt((2 * blockSize) / inLen))
		__prevInputs.pop_back();

	// Make x contain the current input to the right and previous inputs to the left of it
	int i = 1;
	for (auto prev : __prevInputs)
	{
		auto prevp = prev.getArrayOfReadPointers();

		for (int j = 0; j < inLen; j++)
		{
			if (i*inLen > 2 * blockSize)
				continue;

			xp[0][(2 * blockSize - i * inLen) + j] = prevp[0][j];
			xp[1][(2 * blockSize - i * inLen) + j] = prevp[1][j];
		}

		i++;
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
		float wetOutLeft = convOutputp[0][i + 2 * blockSize - inLen];
		float wetOutRight = convOutputp[1][i + 2 * blockSize - inLen];

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
	__inputBlocks.clear();
	__prevInputs.clear();
	__emptyCounter = 0;
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

