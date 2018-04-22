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

#ifndef PIPELINE_H
#define PIPELINE_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeGenerator.h"
#include "WavetableOsc.h"
#include "DelayEffect.h"
#include "LFO.h"
#include "FilterLP.h"
#include "FilterHP.h"
#include "ConvolutionReverb.h"
#include "DistEffect.h"

#include <tuple>
#include <array>

template<typename T>
class Pipeline 
{
	typedef std::tuple<IGenerator*, AudioParameterFloat*, AudioParameterBool*> OscTripple;
private:
	static const size_t __num_osc = 4;
	static const size_t __num_effects = 3;
	std::array<OscTripple, __num_osc> __oscs;
	std::array<IEffect<T>*, __num_osc*__num_effects> __effects;
	AudioBuffer<T> tmpBuff;
	double __rate;
	int __note;
	bool __active;
	const int __maxBuffHint;

public:


	bool isActive();

	void midiCommand(MidiMessage msg, int offset);
	void forceMidiCommand(MidiMessage msg);


	Pipeline(double rate,int maxBuffHint,GLOBAL*global);
	Pipeline(const Pipeline<T>&) = delete;
	Pipeline(Pipeline<T>&&);
	int getNoteNumber();
	void render_block(AudioBuffer<T>& buffer,int len);

	~Pipeline();
	static void RegisterParameters(int ID, GLOBAL*Global);

	void Reset();
	GLOBAL * Global;

};

#endif