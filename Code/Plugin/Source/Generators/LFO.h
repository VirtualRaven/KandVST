#ifndef LFO_H
#define LFO_H
#include "Wavetable.h"
#include "Global.h"
const unsigned int LFO_COUNT = 2;

class LFO
{
private:
	double __phase;
	double __sampleRate;
	int __ID;
	double* __samples;
	double __squareTable[4096];
	int __nrOfSamples;
	bool __activeCheck;
	//double* __parameter; // TEMPORARY
	const IWavetable* __wavetable;
	AudioParameterFloat* __amount;
	AudioParameterBool* __isActive;
	AudioParameterInt* __ratio;
	AudioParameterInt* __waveType;

	double calcRatio();

	~LFO();
	LFO(const LFO&) = delete;
public:

	LFO(int maxSamples, int ID, double sampleRate, GLOBAL*global);
	static void RegisterParameters(int ID, GLOBAL*Global);
	void generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo);
	double* getPointer();
	float getAmount();
	bool isActive();
	GLOBAL* Global;

};
extern LFO* lfos[LFO_COUNT];
#endif //!LFO_H