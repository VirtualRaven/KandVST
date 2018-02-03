#ifndef ENVELOPE_GENERATOR_H
#define ENVELOPE_GENERATOR_H
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


	double calculateSlope(double begin, double end, double time);


public:
	void Reset();
	double GenerateNextStep(bool sustain);
	EnvelopeGenerator(double sampleRate);
	~EnvelopeGenerator();
};
#endif //ENVELOPE_GENERATOR_H

