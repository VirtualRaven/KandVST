#ifndef ENVELOPE_GENERATOR_H
#define ENVELOPE_GENERATOR_H
#include "ParameterHandler.h"

#include "Global.h"
class EnvelopeGenerator
{
private:
	double __sampleRate;
	double __sAttack;
	double __sDecay;
	double __sRelease;
	double __sSustain;
	int __Attack;
	int __Decay;
	int __Release;
	int __state;
	int __sustain;
	double __amplitude;
	AudioParameterFloat* __pattack;
	AudioParameterFloat* __pdecay;
	AudioParameterFloat* __psustain;
	AudioParameterFloat* __prelease;


	double calculateSlope(double begin, double end, double time);
	void recalculateParameters();

public:
	void Reset();
	double GenerateNextStep(bool sustain);
	EnvelopeGenerator(double sampleRate);
	~EnvelopeGenerator();
};
#endif //ENVELOPE_GENERATOR_H

