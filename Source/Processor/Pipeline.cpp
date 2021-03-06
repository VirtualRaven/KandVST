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

#include "Pipeline.h"
#include <math.h>

template<typename T>
Pipeline<T>::Pipeline(double rate,int maxBuffHint,GLOBAL*global) :
	__rate(rate),
	__active(false),
	__maxBuffHint(maxBuffHint),
	Global(global)
{
	for (int i = 0; i < this->__num_osc; i++) {
		__oscs[i] = std::make_tuple(
				new WavetableOsc(i, rate,maxBuffHint,Global),
				Global->paramHandler->Get<AudioParameterFloat>(i, "OSC_MIX_AMP"),
				Global->paramHandler->Get<AudioParameterBool>(i, "OSC_MIX_EN")
			);

		__effects[i*__num_effects] = new DistEffect<T>(i, rate,Global);
		__effects[i*__num_effects + 1] = new FilterLP<T>(i, rate, "FILTER_LP", Global);
		__effects[i*__num_effects + 2] = new FilterHP<T>(i, rate, "FILTER_HP", Global);
	}

	tmpBuff.setSize(2, maxBuffHint);

}


template<typename T>
int Pipeline<T>::getNoteNumber()
{
	return __note;
}

template<typename T>
void Pipeline<T>::midiCommand(MidiMessage msg, int offset)
{
	if (msg.isNoteOn())
	{
		__note = msg.getNoteNumber();
		__active = true;
		for (auto lfo : Global->lfos)
			lfo->keyPressed();
	}
	
	if (__active) {
		for (auto obj : __oscs)
		{
			if (*std::get<2>(obj))
			{
				std::get<0>(obj)->AddCommand(msg, offset);

			}
		}
	}
}

template<typename T>
void Pipeline<T>::forceMidiCommand(MidiMessage msg)
{
	if (__active)
		return;

	for (auto osc : __oscs)
	{
		std::get<0>(osc)->ProccessCommand(msg);
	}
}

template<typename T>
Pipeline<T>::Pipeline(Pipeline<T>&& ref) :
__rate(ref.__rate),
__active(ref.__active),
Global(ref.Global),
__maxBuffHint(ref.__maxBuffHint){
	for (size_t i = 0; i < this->__num_osc; i++) {
		__oscs[i] = ref.__oscs[i];
		for (size_t j = 0; j < this->__num_effects; j++) {
			__effects[i*__num_effects + j] = ref.__effects[i*__num_effects + j];
			ref.__effects[i*__num_effects + j] = nullptr;
		}
		ref.__oscs[i] = std::make_tuple(nullptr, nullptr, nullptr);
	}

}

template<typename T>
Pipeline<T>::~Pipeline()
{
	for (auto&& obj : __oscs) {
		auto && tmp = std::get<0>(obj);
		delete tmp;
		tmp = nullptr;
	}

	for (auto&& obj : __effects) {
		delete obj;
	}

}

template<typename T>
bool Pipeline<T>::isActive() {
	return __active;
}

template<typename T>
void Pipeline<T>::render_block(AudioBuffer<T>& buffer,int len) {
	
	if (__active) {
		if (len > tmpBuff.getNumSamples()) {
			tmpBuff.setSize(2, len, false, false, true);
		}

		this->tmpBuff.clear(0, len);
		bool soundGenerated = false;
		bool oscActive = false;
		for (int i = 0; i < __num_osc; i++) {
			auto obj = __oscs[i];
			//Is the osc active?
			if (*std::get<2>(obj)) {

				//Run osc and check if they generated anything?
				bool osc = std::get<0>(obj)->RenderBlock(tmpBuff, len);
				oscActive |= osc;
				bool effectGenSound = false;
				//Apply the effects
				for (int j = 0; j < __num_effects; j++) {
					effectGenSound |= __effects[i*__num_effects + j]->RenderBlock(tmpBuff, len, !osc);
				}

				//Run copy rutine if the effects or osc produced any sound
				if (osc || effectGenSound) {
					buffer.addFrom(0, 0, tmpBuff, 0, 0, len, *std::get<1>(obj));
					buffer.addFrom(1, 0, tmpBuff, 1, 0, len, *std::get<1>(obj));
					this->tmpBuff.clear(0, len);
					soundGenerated = true;
				}

			}
		}

		if (!oscActive && !soundGenerated)
			__active = false;
		else
			__active = true;
	}
}


template<typename T>
void Pipeline<T>::RegisterParameters(int ID, GLOBAL*Global)
{
	for (size_t i = 0; i < Pipeline::__num_osc; i++)
	{
		Global->paramHandler->RegisterBool(Pipeline::__num_osc*ID + i, "OSC_MIX_EN", "Enable Oscillator", false);
		Global->paramHandler->RegisterFloat(Pipeline::__num_osc * ID + i, "OSC_MIX_AMP", "Oscillator Amplitude", 0.0f, 1.0f, 0.5f);
	}
}

template<typename T>
void Pipeline<T>::Reset()
{
	__active = false;
	for (auto e : __effects)
	{
		e->Reset();
	}
	for (auto& o : __oscs)
	{
		std::get<0>(o)->Reset();
	}
}

template class Pipeline<double>;
template class Pipeline<float>;