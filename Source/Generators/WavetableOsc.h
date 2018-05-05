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

#ifndef WAVETABLEOSC_H
#define WAVETABLEOSC_H

#include "IGenerator.h"
#include "EnvelopeGenerator.h"
#include "IWavetable.h"
#include "IVSTParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Swatch.h"
#include "ThemePicker.h"
#include "OurLookAndFeel.h"


class WavetableOsc : public IGenerator, public IVSTParameters
{
private:
	int __maxBuffHint;
	EnvelopeGenerator __envelope;
	int __note;
	double __frequency;
	double __phase;
	bool __sustainPedal;
	bool __keyDown;

	AudioParameterInt* __waveType, *__octave,* __offset, *__overtone, *__pitchBendSens;
	AudioParameterFloat* __detune, *__sinAmp,* __sqAmp, *__sawAmp, *__triAmp,*__noiseAmp, *__panning;
	AudioParameterChoice* __lfofreq, *__lfoamp;
	float __pitchbend;
	template<typename T>
	bool __RenderBlock(AudioBuffer<T>& buffer,int len);

	double * __envBuff;

	//Random things
	std::vector<double> __noiseBuffer;
	Random __rand;
	size_t __rand_index;

	std::array<double, IWavetable::__length> __noiseData;

public:
	OurLookAndFeel ourLookAndFeel;
	WavetableOsc(int ID,double sampleRate,int __maxBuffHint,GLOBAL*global);
	WavetableOsc(const WavetableOsc&) = delete;
	WavetableOsc(WavetableOsc&& ref);
	virtual ~WavetableOsc();

	// Inherited via Generator
	virtual void ProccessCommand(MidiMessage msg) override;
	static void RegisterParameters(int ID, GLOBAL*Global);
	virtual bool RenderBlock(AudioBuffer<float>& buffer,int len) override
	{
		return __RenderBlock(buffer, len);
	}
	virtual bool RenderBlock(AudioBuffer<double>& buffer, int len) override
	{
		return __RenderBlock(buffer, len);
	}

	virtual const char * name() const override {
		return "Wavetable osc";
	}
	void renderImage(Image* image);

	virtual void Reset() override;
	GLOBAL* Global;
	ThemePicker __themePicker;
	JUCE_LEAK_DETECTOR(WavetableOsc);
};

#endif //!WAVETABLEOSC_H

