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

#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <fstream>
#include "Pipeline.h"
#include <list>
#include <array>
#include "LFO.h"
#include "FilterHP.h"
#include "FilterLP.h"
#include "ConvolutionReverb.h"
#include "BitcrushEffect.h"
#include <memory>

template<typename T>
class PipelineManager
{
private:
	static const size_t NUM_PIPELINES = 16;
	double __sampleRate;
	int   __maybeMaxBuff;
	std::vector<Pipeline<T>> pipList;
	std::array<AudioBuffer<T>,NUM_PIPELINES> pipBuff;
	std::array<std::unique_ptr<LFO>, 2> lfos;
	AudioParameterFloat* __masterGain;
	ThreadPool pool;
	FilterHP<T> __filterHP;
	FilterLP<T> __filterLP;
	DelayEffect<T>__delay;
	BitcrushEffect<T> __bitcrush;
	ConvolutionReverb<T> __reverb;
public:
	PipelineManager(double rate, int maxBuffHint,GLOBAL*global);
	~PipelineManager();	
	static void RegisterParameters(int ID, GLOBAL*Global);
	void Reset();
	GLOBAL * Global;

void genSamples(
		AudioBuffer<T>& buff, 
		MidiBuffer& midiMessages,
		AudioPlayHead::CurrentPositionInfo & posInfo);


};

#endif //PIPELINE_MANAGER_H

