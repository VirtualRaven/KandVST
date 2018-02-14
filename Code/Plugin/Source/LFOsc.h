#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"
#include "IVSTParameters.h"

class LFO
{
private:
	double __phase;
	double __sampleRate;
	//double* __parameter;; // TEMPORARY
	const IWavetable* __wavetable;
public:
	AudioParameterFloat* amount;
	AudioParameterBool* isActive;
	AudioParameterInt* ratio;
	AudioParameterInt* bpm;
	LFO(int bpm, double sampleRate, int ID);
	void apply(double& tmp);
	//void setRatio(int ratio);
	//void setAmount(double amount);
	//void setBpm(int bpm);
};

#endif //!LFO_H