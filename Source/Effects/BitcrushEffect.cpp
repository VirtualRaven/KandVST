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

#include "BitcrushEffect.h"

template<typename T>
BitcrushEffect<T>::BitcrushEffect(int ID, double sampleRate, GLOBAL * global) :
	IEffect<T>(sampleRate),
	IVSTParameters(ID),
	__step(0.0)
{
	Global = global;
	__isEnabled = global->paramHandler->Get<AudioParameterBool>(ID, "BITCRUSH_EN");
	__bits = global->paramHandler->Get<AudioParameterInt>(ID, "BITCRUSH_BITS");
}

template<typename T>
BitcrushEffect<T>::~BitcrushEffect()
{
}

template<typename T>
void BitcrushEffect<T>::RegisterParameters(int ID, GLOBAL* Global)
{
	Global->paramHandler->RegisterBool(ID, "BITCRUSH_EN", "BITCRUSH", 0);
	Global->paramHandler->RegisterInt(ID, "BITCRUSH_BITS", "Bits", 1, 16, 15);
}

template<typename T>
bool BitcrushEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	// Check if enabled
	if (*__isEnabled == false)
		return false;

	__step = 1.0 / (std::pow(2.0, *__bits));

	for (int i = 0; i < len; i++)
	{
		buffer.setSample(0, i, __step * std::floor(buffer.getSample(0, i) / __step + 0.5));
		buffer.setSample(1, i, __step * std::floor(buffer.getSample(1, i) / __step + 0.5));
	}

	return true;
}

template<typename T>
void BitcrushEffect<T>::ProccessCommand(MidiMessage message)
{
}

template<typename T>
void BitcrushEffect<T>::Reset()
{
	
}

template class BitcrushEffect<double>;
template class BitcrushEffect<float>;

