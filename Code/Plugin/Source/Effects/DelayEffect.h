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

#ifndef DELAY_EFFECT_H
#define DELAY_EFFECT_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "IVSTParameters.h"

template<typename T>
class DelayEffect :
	public IEffect<T>,
	public IVSTParameters
{

private:


	AudioBuffer<T> __delayBuffer;
	int __delayLen;
	int __prevDelayLen;
	int __delayPos;
	double __bps; // beats per second
	bool __prevStatus;
	AudioParameterFloat* __delayLenMult;
	AudioParameterFloat* __delayMultiplier;
	AudioParameterBool* __isEnabled;
	AudioParameterBool* __sync;
	
public:
	DelayEffect(int ID,double sampleRate,GLOBAL*global);
	~DelayEffect();
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	static void RegisterParameters(int ID, GLOBAL*Global);
	void setStatus(double bpm,bool status);

	virtual void ProccessCommand(MidiMessage message) override;
	virtual void Reset() override;
	GLOBAL * Global;
};


#endif //DELAY_EFFECT_H
