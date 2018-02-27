#ifndef ENVELOPE_GENERATOR_H
#define ENVELOPE_GENERATOR_H
#include "ParameterHandler.h"
#include "IVSTParameters.h"
#include "Global.h"
class EnvelopeGenerator: public IVSTParameters
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
	uint8 __vel;
	double __velMulti;
	AudioParameterFloat* __pattack;
	AudioParameterFloat* __pdecay;
	AudioParameterFloat* __psustain;
	AudioParameterFloat* __prelease;


	double calculateSlope(double begin, double end, double time);
	void recalculateParameters();

public:
	void Reset(uint8 vel);
	double GenerateNextStep(bool sustain);
	static void RegisterParameters(int ID);
	EnvelopeGenerator(int ID,double sampleRate);
	bool isActive() const;
	~EnvelopeGenerator();
};
#endif //ENVELOPE_GENERATOR_H

