#include "EnvelopeGenerator.h"
#define S_LEVEL 0.5
#define S_ENDLEVEL 0.0
#define A_LEVEL 1.0



double EnvelopeGenerator::GenerateNextStep(bool sustain)
{
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
	return __amplitude;
}

void EnvelopeGenerator::Reset() {
	__state = 0;
}


double EnvelopeGenerator::calculateSlope(double begin,double end, double time) {
	if (time == 0)
		return 0;
	return (end - begin) / time;
}

EnvelopeGenerator::EnvelopeGenerator(double sampleRate):
	__sampleRate(sampleRate),
	__sAttack(0.05),
	__sDecay(0.1),
	__sRelease(0.3),
	__sSustain(1.0),
	__amplitude(0)
{
	__Attack = __sAttack * sampleRate;
	__Decay = __sDecay * sampleRate;
	__Release = __sRelease * sampleRate;
	__sustain = __sSustain * sampleRate;

}


EnvelopeGenerator::~EnvelopeGenerator()
{
}
