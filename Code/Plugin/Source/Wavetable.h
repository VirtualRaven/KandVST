#ifndef WAVETABLE_H
#define WAVETABLE_H
#include <vector>
#include <math.h>
#include "IWavetable.h"
#include "../JuceLibraryCode/JuceHeader.h"

const int size = 2048;

class SinWavetable : public IWavetable
{
public:
	SinWavetable(int length);
	virtual ~SinWavetable();
	virtual double getSample(int idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(int idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}
private:
	template<typename T>
	T __getSample(int idx, T frequency) const;
	double __table[size];
};

class SquareWavetable : public IWavetable
{
public:
	SquareWavetable(int size, double sampleRate);
	virtual ~SquareWavetable();
	virtual double getSample(int idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(int idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}
private:
	template<typename T>
	T __getSample(int idx, T frequency) const;
	const int __nrOfTables = 10;
	double __tables[10][size];
};

class SawWavetable : public IWavetable
{
public:
	SawWavetable(int size, double sampleRate);
	virtual ~SawWavetable();
	virtual double getSample(int idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(int idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}
private:
	template<typename T>
	T __getSample(int idx, T frequency) const;
	const int __nrOfTables = 10;
	double __tables[10][size];
};

class TriangleWavetable : public IWavetable
{
public:
	TriangleWavetable(int size, double sampleRate);
	virtual ~TriangleWavetable();
	virtual double getSample(int idx, double frequency) const
	{
		return __getSample(idx, frequency);
	}
	virtual float getSample(int idx, float frequency) const
	{
		return __getSample(idx, frequency);
	}
private:
	template<typename T>
	T __getSample(int idx, T frequency) const;
	const int __nrOfTables = 10;
	double __tables[10][size];
};



#endif

