#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"

class LFO
{
private:
	bool __isActive;
	double __phase;
	double __sampleRate;
	double* __parameter;
	int __ratio;
	int __bpm; // TEMPORARY
	const IWavetable* __wavetable;
public:
	LFO(int bpm, double* parameter, double sampleRate);
	void apply(double& tmp);
	void setRatio();
};

#endif //!LFO_H