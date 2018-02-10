#include "Wavetable.h"
#include <iostream>
#include <fstream>

/*
* Sinewave
*/

SinWavetable::SinWavetable(int size)
{
	__length = size;
	for (int i = 0; i < size; i++)
	{
		__table[i] = sin(double_Pi * 2 * i / size);
	}
}
SinWavetable::~SinWavetable()
{

}
template<typename T>
T SinWavetable::__getSample(int idx, T frequency) const
{
	return __table[idx % __length];
}


template double SinWavetable::__getSample(int idx, double frequency) const;
template float SinWavetable::__getSample(int idx, float frequency) const;

/*
* Squarewave
*/

SquareWavetable::SquareWavetable(int size, double sampleRate)
{

	__length = size;
	double omega = double_Pi * 2 / __length;
	//int nrOfharms = sampleRate / 2 / 20 * j;
	for (int k = 0; k < __nrOfTables; k++)
	{
		for (int i = 0; i < __length; i++)
		{
			__tables[k][i] = 0.0;
		}


		int nrOfHarms = floor(sampleRate / (2 * 20 * pow(2, k)));
		for (int i = 1; i < nrOfHarms; i++)
		{
			double bk = ((-2) / (double_Pi * i)) * (i % 2);
			double tmp = omega * i;
			for (int j = 0; j < __length; j++)
			{
				__tables[k][j] += (sin(tmp * j) * bk) * 2;
			}
		}
	}
}
SquareWavetable::~SquareWavetable()
{

}
template<typename T>
T SquareWavetable::__getSample(int idx, T frequency) const
{
	int tableNr = floor(log2(frequency / 20)); // 20 = minimum frequency
	if (tableNr > __nrOfTables) tableNr = __nrOfTables-1;
	return __tables[tableNr][idx % __length];
}
template double SquareWavetable::__getSample(int idx, double frequency) const;
template float SquareWavetable::__getSample(int idx, float frequency) const;

/*
 * Sawtoothwave
 */

SawWavetable::SawWavetable(int size, double sampleRate)
{

	__length = size;
	double omega = double_Pi * 2 / __length;
	//int nrOfharms = sampleRate / 2 / 20 * j;
	for (int k = 0; k < __nrOfTables; k++)
	{
		for (int i = 0; i < __length; i++)
		{
			__tables[k][i] = 0.0;
		}

		int nrOfHarms = floor(sampleRate / (2 * 20 * pow(2, k)));
		for (int i = 1; i < nrOfHarms; i++)
		{
			double bk = (-2) / (double_Pi * i);
			double tmp = omega * i;
			for (int j = 0; j < __length; j++)
			{
				__tables[k][j] += sin(tmp * j) * bk;
			}
		}
	}
}
SawWavetable::~SawWavetable()
{

}
template<typename T>
T SawWavetable::__getSample(int idx, T frequency) const
{
	int tableNr = floor(log2(frequency / 20)); // 20 = minimum frequency
	if (tableNr > __nrOfTables) tableNr = __nrOfTables-1;
	return __tables[tableNr][idx % __length];
}

template double SawWavetable::__getSample(int idx, double frequency) const;
template float SawWavetable::__getSample(int idx, float frequency) const;

/*
* Trianglewave
*/

TriangleWavetable::TriangleWavetable(int size, double sampleRate)
{

	__length = size;
	double omega = double_Pi * 2 / __length;
	//int nrOfharms = sampleRate / 2 / 20 * j;
	for (int k = 0; k < __nrOfTables; k++)
	{
		for (int i = 0; i < __length; i++)
		{
			__tables[k][i] = 0.0;
		}

		int nrOfHarms = floor(sampleRate / (2 * 20 * pow(2, k)));
		for (int i = 1; i < nrOfHarms; i++)
		{
			double ak = ((-2*(-4)) / pow((double_Pi * i),2)) * (i % 2);
			double tmp = omega * i;
			int offset =  __length / 4;
			for (int j = 0; j < __length; j++)
			{
				__tables[k][j] += cos(tmp * (j - offset)) * ak;
			}
		}
	}
}
TriangleWavetable::~TriangleWavetable()
{

}
template<typename T>
T TriangleWavetable::__getSample(int idx, T frequency) const
{
	int tableNr = floor(log2(frequency / 20)); // 20 = minimum frequency
	if (tableNr > __nrOfTables) tableNr = __nrOfTables-1;
	return __tables[tableNr][idx % __length];
}

template double TriangleWavetable::__getSample(int idx, double frequency) const;
template float TriangleWavetable::__getSample(int idx, float frequency) const;



const IWavetable* tables[WAVE_TYPE::__COUNT] = { nullptr,nullptr,nullptr,nullptr };
void populateWavetable(double sampleRate)
{
	freeWavetable();
	int tmpSize = 2048;
	tables[WAVE_TYPE::SINE] = new SinWavetable(tmpSize);
	tables[WAVE_TYPE::SQUARE] = new SquareWavetable(tmpSize, sampleRate);
	tables[WAVE_TYPE::SAW] = new SawWavetable(tmpSize, sampleRate);
	tables[WAVE_TYPE::TRI] = new TriangleWavetable(tmpSize, sampleRate);
}

void freeWavetable() {
	for (int i = 0; i < WAVE_TYPE::__COUNT; i++) {
		delete tables[i];
		tables[i] = nullptr;
	}
}
