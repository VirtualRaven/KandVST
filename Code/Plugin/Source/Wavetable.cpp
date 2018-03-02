#include "Wavetable.h"
#include <iostream>
#include <fstream>
#include "../JuceLibraryCode/JuceHeader.h"


static ThreadPool wavePool;

static volatile bool tableRdy[] = { false,false,false,false };

/*
* Sinewave
*/


SinWavetable::SinWavetable()
{
	for (int i = 0; i < __length; i++)
	{
		__tables[0][i] = sin(double_Pi * 2 * i / __length);
	}
}
SinWavetable::~SinWavetable()
{

}
template<typename T>
T SinWavetable::__getSample(double idx, T ) const
{
	double diff = floor(idx) -idx;
	int i = static_cast<int>(idx);
	return static_cast<T>(__tables[0][i % __length] * (1-diff) + __tables[0][(i+1) % __length] * diff);
}


template double SinWavetable::__getSample(double idx, double frequency) const;
template float SinWavetable::__getSample(double idx, float frequency) const;

/*
* Squarewave
*/

SquareWavetable::SquareWavetable(double sampleRate)
{
	double omega = double_Pi * 2 / __length;
	//int nrOfharms = sampleRate / 2 / 20 * j;
	for (int k = 0; k < __NrTables; k++)
	{
		for (int i = 0; i < __length; i++)
		{
			__tables[k][i] = 0.0;
		}


		int nrOfHarms = static_cast<int>(floor(sampleRate / (2 * 20 * pow(2, k))));
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
T SquareWavetable::__getSample(double idx, T frequency) const
{
	int tableNr = frequency < 20 ? 0 : static_cast<int>(floor(log2(frequency / 20))); 
	if (tableNr > __NrTables) tableNr = __NrTables-1;
	double diff = floor(idx) - idx;
	int i = static_cast<int>(idx);
	return static_cast<T>(__tables[tableNr][i % __length] * (1.0-diff) + __tables[tableNr][(i+1) % __length] * diff);
}
template double SquareWavetable::__getSample(double idx, double frequency) const;
template float SquareWavetable::__getSample(double idx, float frequency) const;

/*
 * Sawtoothwave
 */

SawWavetable::SawWavetable( double sampleRate)
{


	double omega = double_Pi * 2 / __length;
	//int nrOfharms = sampleRate / 2 / 20 * j;
	for (int k = 0; k < __NrTables; k++)
	{
		for (int i = 0; i < __length; i++)
		{
			__tables[k][i] = 0.0;
		}

		int nrOfHarms = static_cast<int>(floor(sampleRate / (2 * 20 * pow(2, k))));
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
T SawWavetable::__getSample(double idx, T frequency) const
{
	int tableNr = frequency < 20 ? 0 : static_cast<int>(floor(log2(frequency / 20)));
	if (tableNr > __NrTables) tableNr = __NrTables-1;
	double diff = floor(idx) - idx;
	int i = static_cast<int>(idx);
	return static_cast<T>(__tables[tableNr][i % __length] * (1.0 - diff) + __tables[tableNr][(i + 1) % __length] * diff);
}

template double SawWavetable::__getSample(double idx, double frequency) const;
template float SawWavetable::__getSample(double idx, float frequency) const;

/*
* Trianglewave
*/

TriangleWavetable::TriangleWavetable( double sampleRate)
{

	
	double omega = double_Pi * 2 / __length;
	//int nrOfharms = sampleRate / 2 / 20 * j;
	for (int k = 0; k < __NrTables; k++)
	{
		for (int i = 0; i < __length; i++)
		{
			__tables[k][i] = 0.0;
		}

		int nrOfHarms = static_cast<int>(floor(sampleRate / (2 * 20 * pow(2, k))));
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
T TriangleWavetable::__getSample(double idx, T frequency) const
{
	int tableNr = frequency < 20 ? 0 : static_cast<int>(floor(log2(frequency / 20)));
	if (tableNr > __NrTables) tableNr = __NrTables-1;
	double diff = floor(idx) - idx;
	int i = static_cast<int>(idx);
	return static_cast<T>(__tables[tableNr][i % __length] * (1.0 - diff) + __tables[tableNr][(i + 1) % __length] * diff);
}

template double TriangleWavetable::__getSample(double idx, double frequency) const;
template float TriangleWavetable::__getSample(double idx, float frequency) const;



const IWavetable* tables[WAVE_TYPE::__COUNT] = { nullptr,nullptr,nullptr,nullptr };
void populateWavetable(double sampleRate)
{
	freeWavetable();

	std::function<void()> jobs[] = {
		[]() {tables[WAVE_TYPE::SINE] = new SinWavetable(); tableRdy[0] = true; },
		[sampleRate]() {tables[WAVE_TYPE::SQUARE] = new SquareWavetable(sampleRate); tableRdy[1] = true; },
		[sampleRate]() {tables[WAVE_TYPE::SAW] = new SawWavetable(sampleRate); tableRdy[2] = true; },
		[sampleRate]() {tables[WAVE_TYPE::TRI] = new TriangleWavetable(sampleRate); tableRdy[3] = true; }
	};

	for (auto&& j : jobs) {
		wavePool.addJob(j);
	}
#ifdef NO_ASYNC_GEN
	while (!wavetableRdy());
#endif //NO_ASYNC_GEN 

}

void freeWavetable() {
	for (int i = 0; i < WAVE_TYPE::__COUNT; i++) {
		delete tables[i];
		tables[i] = nullptr;
		tableRdy[i] = false;
	}
}

bool wavetableRdy() {
	auto acc = true;
	for (auto tmp : tableRdy) {
		acc &= tmp;
	}
	return acc;
}
