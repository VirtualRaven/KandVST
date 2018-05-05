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

#ifndef ENVELOPE_GENERATOR_H
#define ENVELOPE_GENERATOR_H
#include "ParameterHandler.h"
#include "IVSTParameters.h"
#include "Global.h"
#include <math.h>
#include "Swatch.h"
#include "ThemePicker.h"
class EnvelopeGenerator: public IVSTParameters
{
private:
	double __sampleRate;
	int __state;
	double __amplitude;
	uint8 __vel;
	uint8 __sustain;
	double __velMulti;
	AudioParameterFloat * __a_time,*__a_level, *__a_curve, *__h_time, *__d_time, *__d_level, *__d_curve, *__r_time, *__r_curve,*__s_level,*__s_time,*__s_curve;
	

	int __counter;

	double calculateSlope(double begin, double end, double time);
	void recalculateParameters();

public:
	void Reset(uint8 vel);
	double GenerateNextStep(bool sustain);
	void RenderBlock(double * buffer, size_t size);
	void setVelocity(uint8 vel);
	void setSustain(uint8 sust);
	static void RegisterParameters(int ID, GLOBAL*Global);
	EnvelopeGenerator(int ID,double sampleRate,GLOBAL*global);
	bool isActive() const;
	void Stop();
	static void RenderImage(int __ID, Image * image, GLOBAL*Global);
	GLOBAL*Global;
	~EnvelopeGenerator();
};
#endif //ENVELOPE_GENERATOR_H

