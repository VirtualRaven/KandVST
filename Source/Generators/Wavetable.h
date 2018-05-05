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

#ifndef WAVETABLE_H
#define WAVETABLE_H
#include <vector>
#include <math.h>
#include "IWavetable.h"
#include "../JuceLibraryCode/JuceHeader.h"



class SinWavetable : public IWavetable
{
public:
	SinWavetable();
	virtual ~SinWavetable();
	virtual double getSample(double idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(double idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}

private:
	template<typename T>
	T __getSample(double idx, T frequency) const;
};

class SquareWavetable : public IWavetable
{
public:
	SquareWavetable( double sampleRate);
	virtual ~SquareWavetable();
	virtual double getSample(double idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(double idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}

private:
	template<typename T>
	T __getSample(double idx, T frequency) const;
};

class SawWavetable : public IWavetable
{
public:
	SawWavetable( double sampleRate);
	virtual ~SawWavetable();
	virtual double getSample(double idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(double idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}
private:
	template<typename T>
	T __getSample(double idx, T frequency) const;
};

class TriangleWavetable : public IWavetable
{
public:
	TriangleWavetable( double sampleRate);
	virtual ~TriangleWavetable();
	virtual double getSample(double idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(double idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}
private:
	template<typename T>
	T __getSample(double idx, T frequency) const;
};


template<bool B>
inline double getSampleFromLoc(const IWavetable::tableSampleLocation& t, const IWavetable* w) {
	return w->__tables[t.tableNr][t.i_1] * (1.0 - t.diff) + w->__tables[t.tableNr][t.i_2] * t.diff;
}

template<> inline double getSampleFromLoc<false>(const IWavetable::tableSampleLocation& t, const IWavetable*  w) {
	return w->__tables[0][t.i_1] * (1.0 - t.diff) + w->__tables[0][t.i_2] * t.diff;
}

template<WAVE_TYPE T> inline double getSampleFromLoc(const IWavetable::tableSampleLocation& t) {
	return getSampleFromLoc<true>(t, tables[T]);
}
template<> inline double getSampleFromLoc<SINE>(const IWavetable::tableSampleLocation& t) {
	return getSampleFromLoc<false>(t, tables[SINE]);
}





#endif

