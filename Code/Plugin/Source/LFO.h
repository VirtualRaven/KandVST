#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"
#include <vector>

class LFO
{
private:
	double __phase;
	double __sampleRate;
	double __bpm;
	std::vector<double> __samples;
	//double* __parameter; // TEMPORARY
	const IWavetable* __wavetable;
	AudioParameterFloat* __amount;
	AudioParameterBool* __isActive;
	AudioParameterInt* __ratio;
	AudioParameterInt* __waveType;

public:

	LFO(double bpm, double sampleRate, int maxSamples, int ID);
	static void RegisterParameters(int ID);
	void fillBlock(int numSamples, AudioPlayHead::CurrentPositionInfo posInfo);
	double calcRatio();
	double getSample(int idx);
};

#endif //!LFO_H