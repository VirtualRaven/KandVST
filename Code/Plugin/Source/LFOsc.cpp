#include "LFOsc.h"
#include "Wavetable.h"
#include "Log.h"


LFO::LFO(int bpm, double sampleRate, int ID) :
	isActive(false),
	__phase(0.0),
	__wavetable(tables[WAVE_TYPE::SINE]),
	__sampleRate(sampleRate)
{
	//register bpm, ratio, active, amount
}

void LFO::apply(double& tmp)
{
	if (!*isActive) return;
	double freq = (*bpm) * (*ratio) / 60.0;
	double samp = __wavetable->getSample(__phase, freq);
	tmp *= pow(2.0, samp*(*amount) / 12.0);
	__phase += __wavetable->getLength() * freq / __sampleRate;
	
}
/*
void LFO::setAmount(double amount)
{
	//__amount = amount;
}
void LFO::setRatio(int ratio)
{
	__ratio = ratio;
}
void LFO::setBpm(int bpm)
{
	__bpm = bpm;
}
*/