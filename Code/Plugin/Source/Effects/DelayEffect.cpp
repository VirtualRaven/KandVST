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

#include "DelayEffect.h"
#define FADE 0.3

template<typename T>
DelayEffect<T>::DelayEffect(int ID, double sampleRate,GLOBAL*global) :
	IEffect<T>(sampleRate),
	IVSTParameters(ID),
	__delayBuffer(2, static_cast<int>(sampleRate * 4)),	// Buffer size is max delay
	__delayLen(0),	// Actual delay length
	__prevDelayLen(0),
	__delayPos(0),
	__bps(0.0),
	__prevStatus(false)
{
	Global = global;
	__isEnabled = global->paramHandler->Get<AudioParameterBool>(ID, "DELAY_EN");
	__sync = global->paramHandler->Get<AudioParameterBool>(ID, "DELAY_SYNC");
	__delayMultiplier = Global->paramHandler->Get<AudioParameterFloat>(ID, "EX_DELAYMULTI");
	__delayLenMult = Global->paramHandler->Get<AudioParameterFloat>(ID, "EX_DELAYLENGTH");

	for (int i = 0; i < __delayBuffer.getNumSamples(); i++)
	{
		__delayBuffer.setSample(0, i, 0);
		__delayBuffer.setSample(1, i, 0);
	}
}

template<typename T>
DelayEffect<T>::~DelayEffect()
{
}
template<typename T>
void DelayEffect<T>::setStatus(double bpm, bool status) {
	__bps = bpm >= 60.0 ? bpm / 60.0 : 1.0;
	if (__prevStatus != status)
	{
		//Clear buffer at starting or stopping the track
		__delayBuffer.clear();
	}
	__prevStatus = status;
}
template<typename T>
void DelayEffect<T>::RegisterParameters(int ID,GLOBAL*Global)
{
	Global->paramHandler->RegisterBool(ID, "DELAY_EN", "DELAY", 0);
	Global->paramHandler->RegisterBool(ID, "DELAY_SYNC", "Delay sync", 0);
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYMULTI", "Delay", 0.0f, 1.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYLENGTH", "Delay Length", 0.125f, 4.0f, 0.25f);
}

template<typename T>
bool DelayEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	// Check if enabled
	if (*__isEnabled == false)
		return false;

	float multi =*__delayMultiplier;
	float lenmult = (*__delayLenMult);
	
	if (*__sync)
		lenmult *= (1.0 / __bps);

	__delayLen = (this->__sampleRate * lenmult);

	if (__delayLen != __prevDelayLen)
	{
		// Clear buffer when the length has changed
		__delayBuffer.clear();
	}
	__prevDelayLen = __delayLen;

	// Disable when empty
	if (empty) {
		if(__delayBuffer.getMagnitude(0,__delayLen) < 0.0001 )
			return false;
		else {
			auto buff = buffer.getArrayOfWritePointers();
			auto dbuff = __delayBuffer.getArrayOfWritePointers();
			for (int i = 0; i < len; i++)
			{
				dbuff[0][__delayPos] *=multi ;
				dbuff[1][__delayPos] *=multi ;

				__delayPos = (__delayPos + 1) % __delayLen;

				buff[0][i] += dbuff[0][__delayPos] * multi;
				buff[1][i] += dbuff[1][__delayPos] * multi;
			}
			return true;
		}
	}
	else{
	
	
		auto buff = buffer.getArrayOfWritePointers();
		auto dbuff = __delayBuffer.getArrayOfWritePointers();
		for (int i = 0; i < len; i++)
		{
			dbuff[0][__delayPos] = dbuff[0][__delayPos] * multi + buff[0][i];
			dbuff[1][__delayPos] = dbuff[1][__delayPos] * multi + buff[1][i];

			__delayPos = (__delayPos + 1) % __delayLen;
			
			buff[0][i] += dbuff[0][__delayPos] * multi;
			buff[1][i] += dbuff[1][__delayPos] * multi;
		}

		return true;
	}
}

template<typename T>
void DelayEffect<T>::ProccessCommand(MidiMessage message)
{
}

template<typename T>
void DelayEffect<T>::Reset()
{
	__delayBuffer.clear();
}

template class DelayEffect<double>;
template class DelayEffect<float>;

