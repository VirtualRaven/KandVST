#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"
#include <vector>

const unsigned int LFO_COUNT = 2;

class LFO
{
private:
	double __phase;
	double __sampleRate;
	int __ID;
	std::vector<double> __samples;
	//double* __parameter; // TEMPORARY
	const IWavetable* __wavetable;
	AudioParameterFloat* __amount;
	AudioParameterBool* __isActive;
	AudioParameterInt* __ratio;
	AudioParameterInt* __waveType;

public:

	LFO(int maxSamples, int ID, double sampleRate);
	static void RegisterParameters(int ID);
	void generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo);
	double calcRatio();
	double getSample(int idx);
};
extern LFO* Lfos[LFO_COUNT];
#endif //!LFO_H