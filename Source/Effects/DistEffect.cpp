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

#include "DistEffect.h"
#include "DistEffect.h"
#include "LFO.h"
#include <stdlib.h>

template<typename T>
DistEffect<T>::DistEffect(int ID, double samplerate,GLOBAL*global) : IEffect<T>(samplerate)
{
	Global = global;
	__threshold = Global->paramHandler->Get<AudioParameterFloat>(ID, "DIST_TRSH");
	__isActive = Global->paramHandler->Get<AudioParameterBool>(ID, "DIST_EN");
	__lfoIndex = Global->paramHandler->Get<AudioParameterInt>(ID, "DIST_LFO");
}

template<typename T>
DistEffect<T>::~DistEffect()
{
}

template<typename T>
void DistEffect<T>::RegisterParameters(int ID, GLOBAL*Global)
{
	Global->paramHandler->RegisterFloat(ID, "DIST_TRSH", "Distortion threshold",0.0f,0.95f,0.0f);
	Global->paramHandler->RegisterBool(ID, "DIST_EN", "Distortion enable", false);
	Global->paramHandler->RegisterInt(ID, "DIST_LFO", "Dist lfo", 0, 2, 0);
}

template<typename T>
bool DistEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) {
	if (empty || !(*__isActive)) return false;
	double t = (*__threshold);
	auto buff = buffer.getArrayOfWritePointers();
	if ((*__lfoIndex) > 0) {
		double amount = Global->lfos[(*__lfoIndex)-1]->getAmount();
		if (amount > 0.0) {
			t -= amount * t * ((Global->lfos[(*__lfoIndex) - 1]->getPointer()[0] + 1.0) / 2.0);
		}
	}
	

	for (int i = 0; i < len; i++) {
		double samp0 = buff[0][i];
		double samp1 = buff[1][i];
		int mul0 = samp0 < 0 ? -1 : 1;
		int mul1 = samp1 < 0 ? -1 : 1;
		buff[0][i] = std::min(abs(samp0),1.0-t) * mul0;
		buff[1][i] = std::min(abs(samp1),1.0-t) * mul1;
		
		/*double k = 2.0 * t / (1.0 - t);
		buff[0][i] = (1.0 + k)*samp0 / (1.0 + k*abs(samp0));
		buff[1][i] = (1.0 + k)*samp1 / (1.0 + k*abs(samp1));
		*/
	}
	return true;
}
template<typename T>
void DistEffect<T>::ProccessCommand(MidiMessage message) {

}

template<typename T>
void DistEffect<T>::Reset()
{
}

template class DistEffect<double>;
template class DistEffect<float>;
