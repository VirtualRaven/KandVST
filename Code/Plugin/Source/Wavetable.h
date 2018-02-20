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
	return w->__tables[0][t.i_1] * (1.0 - t.diff) + w->__tables[t.tableNr][t.i_2] * t.diff;
}

template<WAVE_TYPE T> inline double getSampleFromLoc(const IWavetable::tableSampleLocation& t) {
	return getSampleFromLoc<true>(t, tables[T]);
}
template<> inline double getSampleFromLoc<SINE>(const IWavetable::tableSampleLocation& t) {
	return getSampleFromLoc<false>(t, tables[SINE]);
}





#endif

