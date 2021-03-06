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

#include "LFO.h"
#include "Log.h"
#include "Global.h"
#include "ParameterHandler.h"
#include <sstream>

LFO::LFO(int maxSamples, int ID, double sampleRate, GLOBAL*global):
	__wavetable(tables[WAVE_TYPE::SINE]),
	__phase(0.0),
	__ID(ID),
	__sampleRate(sampleRate),
	__activeCheck(false),
	__nrOfSamples(maxSamples),
	__phaseCheck(false)
{
	Global = global;
	__samples = new double[maxSamples];
	__amount	 = Global->paramHandler->Get<AudioParameterFloat>(ID, "LFO_AMOUNT");
	__waveType	 = Global->paramHandler->Get<AudioParameterInt>(ID, "LFO_TYPE");
	__isActive	 = Global->paramHandler->Get<AudioParameterBool>(ID, "LFO_EN");
	__invert	 = Global->paramHandler->Get<AudioParameterBool>(ID, "LFO_INV");
	__onPress	 = Global->paramHandler->Get<AudioParameterBool>(ID, "LFO_PRESS");
	__choice	 = Global->paramHandler->Get <AudioParameterChoice>(ID, "LFO_CHOICE");
}
LFO::~LFO()
{
	delete[] __samples;
}

double LFO::calcRatio()
{
	int indx = (*__choice).getIndex();
	switch (indx) {
	case 0:					// 1/16
		return 16.0;
	case 1:					// 1/8
		return 8.0;
	case 2:					// 1/6
		return 6.0;
	case 3:					// 1/4
		return 4.0;
	case 4:					// 1/3
		return 3.0;
	case 5:					// 1/2
		return 2.0;
	case 6:					// 1
		return 1.0;
	case 7:					// 2
		return 0.5;
	case 8:					// 3
		return 1.0/3.0;
	case 9:					// 4
		return 0.25;
	case 10:				// 6
		return 1.0/6.0;
	case 11:				// 8
		return 0.125;

	}
}

void LFO::RegisterParameters(int ID, GLOBAL*Global)
{
	Global->paramHandler->RegisterBool(ID, "LFO_EN", "LFO", 0);
	Global->paramHandler->RegisterBool(ID, "LFO_INV", "LFO Invert", 0);
	Global->paramHandler->RegisterBool(ID, "LFO_PRESS", "LFO On Press Sync", 0);
	Global->paramHandler->RegisterInt(ID, "LFO_TYPE", "LFO Wave type", 0, 3, 0);
	Global->paramHandler->RegisterFloat(ID, "LFO_AMOUNT", "LFO amount", 0.0, 1.0, 0.5);
	StringArray choices = StringArray("1/16", "1/8", "1/6", "1/4", "1/3", "1/2", "1", "2", "3", "4", "6", "8");
	Global->paramHandler->RegisterChoice(ID, "LFO_CHOICE", "LFO choice", choices, 6);
}

double* LFO::getPointer()
{
	if (!__activeCheck) return nullptr;
	return __samples;
}
float LFO::getAmount()
{
	if (!__activeCheck) return 0.0f;
	return (*__amount);
}
bool LFO::isActive() {
	return __activeCheck;
}
void LFO::keyPressed()
{
	if (*__onPress) __phase = 0.0;
}
void LFO::generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo)
{
	if (numSamples > __nrOfSamples) {
		delete[] __samples;
		__nrOfSamples = numSamples;
		__samples = new double[numSamples];
	}

	if (!(*__isActive)) {
		if (!posInfo.isPlaying && !(*__onPress)) {
			__phase = fmod(__phase + (IWavetable::getLength() * ((posInfo.bpm)* calcRatio() / 60.0) / __sampleRate) * numSamples, IWavetable::getLength());
		}
		__activeCheck = false;
		return;
	}
	//WAVE_TYPE type = toWAVE_TYPE(*__waveType);
	
	double freq = (posInfo.bpm)* calcRatio() / 60.0;

	double inc = IWavetable::getLength() * freq / __sampleRate;

	static int nrLoopsPlayed = 0;
	static double prevPos = posInfo.ppqPosition;


	if (posInfo.isPlaying && !(*__onPress)) {
		if (posInfo.isLooping) {

			if (prevPos > posInfo.ppqPosition) {
				nrLoopsPlayed++;
			}
			__phase = fmod((nrLoopsPlayed * posInfo.timeSigNumerator + posInfo.ppqPosition) * IWavetable::getLength() * calcRatio(), IWavetable::getLength());
		} else 
			__phase = fmod(posInfo.ppqPosition * IWavetable::getLength() * calcRatio(), IWavetable::getLength());
		prevPos = posInfo.ppqPosition;
	}
	else {
		nrLoopsPlayed = 0;
		prevPos = 0;
	}
	int invert = (*__invert) ? -1 : 1;
		// TMPTMPTMP
	switch (toWAVE_TYPE(*__waveType)) {
	case SINE	:
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<SINE>(tgt) * invert;
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	case SQUARE	: 
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<SQUARE>(tgt) * invert;
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	case TRI	: 
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<TRI>(tgt) * invert;
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	case SAW	: 
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<SAW>(tgt) * invert;
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	}

	__activeCheck = true;
}




