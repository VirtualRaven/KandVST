#include "EnvelopeGenerator.h"
#define S_LEVEL 0.5
#define S_ENDLEVEL 0.0
#define A_LEVEL 1.0



double EnvelopeGenerator::GenerateNextStep(bool sustain)
{
	recalculateParameters();

	switch (__state)
	{
	case 0:
		if (!sustain) {
			__state = 3;
			break;
		}

		__amplitude +=calculateSlope(0, A_LEVEL, __Attack);

		if (__amplitude >= A_LEVEL) {
			__state++;
		}
		break;
	case 1:
		if (!sustain) {
			__state = 3;
			break;
		}

		__amplitude +=calculateSlope(A_LEVEL, S_LEVEL, __Decay);

		if (__amplitude <= S_LEVEL) {
			__state++;
		}
		break;
	case 2:
		if (!sustain) {
			__state = 3;
			break;
		}
		if (((S_LEVEL > S_ENDLEVEL)&&(__amplitude > S_ENDLEVEL))||((S_LEVEL < S_ENDLEVEL) && (__amplitude < S_ENDLEVEL))) {
			__amplitude += calculateSlope(S_LEVEL, S_ENDLEVEL,__sustain);
		}
		
		break;
	case 3:

		__amplitude +=calculateSlope(S_LEVEL, 0, __Release);
		if (__amplitude < 0.0) {
			__amplitude = 0.0;
			__state++;
			break;
		}
		break;
	case 4:
		return 0.0;
		break;
	default:
		__state = 0;
		break;
	}
	return __amplitude*__velMulti;
}

void EnvelopeGenerator::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK", "Attack", 0.1f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY", "Decay", 0.1f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN", "Sustain", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_RELEASE", "Release", 0.1f, 2.0f, 0.2f);
}

void EnvelopeGenerator::Reset(uint8 vel) {
	__state = 0;
	__vel = vel;
	__velMulti = vel / 127.0;
}


double EnvelopeGenerator::calculateSlope(double begin,double end, double time) {
	if (time == 0)
		return 0;
	return (end - begin) / time;
}

void EnvelopeGenerator::recalculateParameters()
{
	__Attack = (int)(*__pattack * __sampleRate);
	__Decay = (int)(*__pdecay * __sampleRate);
	__sustain = (int)(*__psustain * __sampleRate);
	__Release = (int)(*__prelease * __sampleRate);

}

EnvelopeGenerator::EnvelopeGenerator(int ID,double sampleRate):
	IVSTParameters(ID),
	__sampleRate(sampleRate),
	__sAttack(0.05),
	__sDecay(0.1),
	__sRelease(0.3),
	__sSustain(1.0),
	__amplitude(0),
	__vel(127),
	__velMulti(1.0)
{
	__pattack =	 Global->paramHandler->Get<AudioParameterFloat>(__ID,"ENV_ATTACK");
	__pdecay =	 Global->paramHandler->Get<AudioParameterFloat>(__ID,"ENV_DECAY");
	__psustain = Global->paramHandler->Get<AudioParameterFloat>(__ID,"ENV_SUSTAIN");
	__prelease = Global->paramHandler->Get<AudioParameterFloat>(__ID,"ENV_RELEASE");
	recalculateParameters();
}


EnvelopeGenerator::~EnvelopeGenerator()
{
}
