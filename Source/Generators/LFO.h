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

#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"
#include "Global.h"
const unsigned int LFO_COUNT = 2;

class LFO
{
private:
	double __phase;
	double __sampleRate;
	int __ID;
	double* __samples;
	int __nrOfSamples;
	bool __activeCheck, __phaseCheck;
	//double* __parameter; // TEMPORARY
	const IWavetable* __wavetable;
	AudioParameterFloat* __amount;
	AudioParameterBool* __isActive, *__invert,*__onPress;
	AudioParameterInt *__waveType;
	AudioParameterChoice* __choice;

	double calcRatio();

	~LFO();
	LFO(const LFO&) = delete;
public:

	LFO(int maxSamples, int ID, double sampleRate, GLOBAL*global);
	static void RegisterParameters(int ID, GLOBAL*Global);
	void generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo);
	double* getPointer();
	float getAmount();
	bool isActive();
	void keyPressed();
	GLOBAL* Global;

};
extern LFO* lfos[LFO_COUNT];
#endif //!LFO_H