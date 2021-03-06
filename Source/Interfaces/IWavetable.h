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

#ifndef IWAVETABLE_H
#define IWAVETABLE_H
#include "JuceHeader.h"
//#define NO_ASYNC_GEN //Comment to enable async generation of wavetabels

class IWavetable
{
	friend class WavetableOsc;
protected:
	static const int __length = 4096;
	static const int __NrTables = 10;
	double __tables[__NrTables][__length];
public:
	static int getLength() 
	{
		return __length;
	}
	virtual double getSample(double phase, double frequency) const = 0;
	virtual float getSample(double phase, float frequency) const = 0 ;
	

        virtual ~IWavetable()=0;

	struct tableSampleLocation{
		int tableNr;
		double diff;
		int i_1;
		int i_2;
	};

	template<bool B> friend  double getSampleFromLoc(const tableSampleLocation& t, const IWavetable* w);

	inline static tableSampleLocation getLoc(double idx, double freq) {
		int tableNr = freq < 20 ? 0 : static_cast<int>(floor(log2(freq / 20)));
		if (tableNr > __NrTables) tableNr = __NrTables - 1;
		double diff = idx - floor(idx);
		int i = static_cast<int>(idx);
		return { tableNr,
				diff,
				i%__length, 
				(i+1)%__length 
		};
	}
};



enum WAVE_TYPE : unsigned int {
	SINE = 0,
	SQUARE ,
	SAW ,
	TRI ,
	__COUNT
};

inline WAVE_TYPE toWAVE_TYPE(int a) {
	switch (a)
	{
		case SINE:
			return SINE;
		case SQUARE:
			return SQUARE;
		case SAW:
			return SAW;
		case TRI:
			return TRI;
		default:
			return SINE;
	}
}

extern const IWavetable* tables[WAVE_TYPE::__COUNT];
extern void populateWavetable(double sampleRate, ThreadPool& wavePool);
extern void freeWavetable();
extern bool wavetableRdy();
#endif //!IWAVETABLE_H
