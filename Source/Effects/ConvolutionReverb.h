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
	GLOBAL* __global;
	int __maxBuffHint;
	double __sampleRate;

	AudioFormatManager __formatManager;

	AudioSampleBuffer __responseBuffer;

	ScopedPointer<dsp::FFT> __fft;
	ScopedPointer<dsp::FFT> __ifft;

	std::vector<AudioSampleBuffer> __responseBlocks;
	std::list<AudioSampleBuffer> __inputBlocks;
	std::list<AudioSampleBuffer> __prevInputs;
	
	int __responseBufferLen;
	int __prevBlockSize;

	AudioParameterBool *__isEnabled;
	AudioParameterFloat *__dryGain;
	AudioParameterFloat *__wetGain;
	AudioParameterChoice *__ir;

	bool __prevIsEnabled;
	bool __prevIsEmpty;
	int __emptyCounter;
	int __blockSizeChangeCounter;
	bool __irFromFile;
	String __prevIrName;

	void __createResponseBlocks(int len);
	void __loadImpulseResponse(ScopedPointer<AudioFormatReader> reader, String errorInfo);

	static String __getExternalIrDir();

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
