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

#ifndef DIST_H
#define DIST_H
#include "IEffect.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"

template<typename T>
class DistEffect :
	public IEffect<T>
{
private:
	AudioParameterFloat* __threshold;
	AudioParameterInt* __lfoIndex;
	AudioParameterBool* __isActive;
public:
	DistEffect(int ID, double samplerate,GLOBAL*global);
	~DistEffect();
	static void RegisterParameters(int ID, GLOBAL*Global);
	bool RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) override;
	virtual void ProccessCommand(MidiMessage message) override;
	virtual void Reset() override;
	GLOBAL * Global;

};
#endif