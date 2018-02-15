#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"
#include "IVSTParameters.h"

class LFO
{
private:
	double __phase;
	double __sampleRate;
	//double* __parameter; // TEMPORARY
	const IWavetable* __wavetable;
	AudioParameterFloat* __amount;
	AudioParameterBool* __isActive;
	AudioParameterInt* __ratio;
	double __bpm;
	double calcRatio();
public:

	LFO(double bpm, double sampleRate, int ID);
	void apply(double& tmp);
	static void RegisterParameters(int ID);
	void setWavetable(int t);
};

#endif //!LFO_H