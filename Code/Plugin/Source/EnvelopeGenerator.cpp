#include "EnvelopeGenerator.h"
#define S_LEVEL 0.5
#define A_LEVEL 1.0



double EnvelopeGenerator::GenerateNextStep(bool sustain)
{
	switch (__state)
	{
	case 0:
		if (!sustain) {
			__state = 3;
			__counter = 0;
			break;
		}

		__amplitude +=calculateSlope(0, A_LEVEL, __Attack);
		__counter++;

		if (__counter >= __Attack) {
			__counter = 0;
			__state++;
		}
		break;
	case 1:
		if (!sustain) {
			__state = 3;
			__counter = 0;
			break;
		}

		__amplitude +=calculateSlope(A_LEVEL, S_LEVEL, __Decay);
		__counter++;

		if (__counter >= __Decay) {
			__counter = 0;
			__state++;
		}
		break;
	case 2:
		if (!sustain) {
			__state = 3;
			__counter = 0;
		}
		__amplitude =  S_LEVEL;
		break;
	case 3:
		if (__counter >= __Release) {
			__counter = 0;
			__state++;
			break;
		}

		__amplitude +=calculateSlope(S_LEVEL, 0, __Release);
		if (__amplitude < 0.0) {
			__amplitude = 0.0;
			__state++;
			__counter = 0;
			break;
		}
		__counter++;


		break;
	case 4:
		return 0.0;
		break;
	default:
		__state = 0;
		__counter = 0;
		break;
	}
	return __amplitude;
}

void EnvelopeGenerator::Reset() {
	__amplitude = 0;
	__state = 0;
	__counter = 0;
}


double EnvelopeGenerator::calculateSlope(double begin,double end, double time) {
	return (end - begin) / time;
}

EnvelopeGenerator::EnvelopeGenerator(double sampleRate):
	__sampleRate(sampleRate),
	__sAttack(0.5),
	__sDecay(0.5),
	__sRelease(0.5),
	__amplitude(0)
{
	__Attack = __sAttack * sampleRate;
	__Decay = __sDecay * sampleRate;
	__Release = __sRelease * sampleRate;

}


EnvelopeGenerator::~EnvelopeGenerator()
{
}
