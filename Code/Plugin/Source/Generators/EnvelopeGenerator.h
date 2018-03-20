#ifndef ENVELOPE_GENERATOR_H
#define ENVELOPE_GENERATOR_H
#include "ParameterHandler.h"
#include "IVSTParameters.h"
#include "Global.h"
#include <math.h>
class EnvelopeGenerator: public IVSTParameters
{
private:
	double __sampleRate;
	int __state;
	double __amplitude;
	uint8 __vel;
	uint8 __sustain;
	double __velMulti;
	AudioParameterFloat * __a_time,*__a_level, *__a_curve, *__h_time, *__d_time, *__d_level, *__d_curve, *__r_time, *__r_curve,*__s_level,*__s_time,*__s_curve;


	int __counter;

	double calculateSlope(double begin, double end, double time);
	void recalculateParameters();

public:
	void Reset(uint8 vel);
	double GenerateNextStep(bool sustain);
	void RenderBlock(double * buffer, size_t size);
	void setVelocity(uint8 vel);
	void setSustain(uint8 sust);
	static void RegisterParameters(int ID);
	EnvelopeGenerator(int ID,double sampleRate);
	bool isActive() const;

	static void RenderImage(int __ID, Image * image);
	~EnvelopeGenerator();
};
#endif //ENVELOPE_GENERATOR_H

