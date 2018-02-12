#include "LFOsc.h"
#include "Wavetable.h"

LFO::LFO(int bpm, double* parameter, double sampleRate) :
	__isActive(true),
	__phase(0.0),
	__parameter(parameter),
	__ratio(1),
	__bpm(120),
	__wavetable(tables[WAVE_TYPE::SINE]),
	__sampleRate(sampleRate)
{
}

void LFO::apply(double& tmp)
{
	double freq = __bpm / 60;
	double samp = __wavetable->getSample(__phase, freq);
	tmp *= pow(2.0, samp*0.5 / 12.0);
	__phase += __wavetable->getLength() * freq / __sampleRate;
}

void LFO::setRatio()
{
}
