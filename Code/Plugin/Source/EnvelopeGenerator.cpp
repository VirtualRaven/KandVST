#include "EnvelopeGenerator.h"
#define S_LEVEL 0.5
#define S_ENDLEVEL 0.0
#define A_LEVEL 1.0



double EnvelopeGenerator::GenerateNextStep(bool sustain)
{
	int __Attack = (int)(*__a_time * __sampleRate);
	int __Decay = (int)(*__d_time * __sampleRate);
	int __sustain = (int)(*__h_time * __sampleRate);
	int __Release = (int)(*__r_time * __sampleRate);

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

void EnvelopeGenerator::RenderBlock(double * buffer, size_t size)
{

	int attackTime = (int)(*__a_time * __sampleRate);
	int holdTime = (int)(*__h_time * __sampleRate);
	int decayTime = (int)(*__d_time * __sampleRate);
	int releaseTime =(int)(*__r_time * __sampleRate);
	int sustainTime =(int)(*__s_time * __sampleRate);

	double a_level = *__a_level * __velMulti;
	double a_c = *__a_curve;
	double a_k = (attackTime)*pow(a_level, -1 / a_c);

	double d_level = *__d_level * __velMulti;
	if (d_level > a_level)
		d_level = a_level;
	double d_c = *__d_curve;
	double d_k = (decayTime)*pow(a_level-d_level, -1 / d_c);

	double s_level = *__s_level * __velMulti;
	if (s_level > d_level)
		s_level = d_level;
	double s_c = *__s_curve;
	double s_k = (sustainTime)*pow(d_level-s_level, -1 / s_c);

	double r_c =*__r_curve;
	double r_k = (releaseTime)*pow(d_level, -1 / r_c);


	//Figure out what settings we gonna use if a reset and amplitude is non-zero
	if (__counter == 0 && __state == 0 && __amplitude > 0.001)
	{
		if (__amplitude<a_level)
		{
			__counter = static_cast<int>(a_k / (pow(__amplitude, -1 / a_c)));
		}
		else if (__amplitude == a_level)
		{
			__state = 1; //Hold state
		}
		else if (__amplitude > a_level) 
		{
			__state = 2; //Decay
			a_level = __amplitude;
		}
	}


	if (__state < 4 && __sustain == 0) {
		__state = 4; //Release
		__counter = static_cast<int>(releaseTime - r_k / (pow(__amplitude, -1 / r_c))); //Calculate were to begin in release
	}

	for (size_t i = 0; i < size; i++)
	{
		switch (__state)
		{
			case 0://Attack
				__amplitude = pow(static_cast<double>(__counter) / a_k, a_c);
				if (__counter>=attackTime)
				{
					__counter = 0;
					__state++;
				}
				break;
			case 1://Hold
				__amplitude = a_level;
				if (__counter>= holdTime)
				{
					__counter = 0;
					__state++;
				}
				break;
			case 2://Decay
				__amplitude = pow(-(static_cast<double>(__counter)- decayTime) / d_k, d_c)+d_level;
				if (__counter>= decayTime)
				{
					__counter = 0;
					__state++;
				}
				if (!isfinite(__amplitude))
					__amplitude = d_level;
				break;
			case 3://Sustain
				if (s_level != d_level) 
				{
					__amplitude = pow(-(static_cast<double>(__counter) - sustainTime) / s_k, s_c) + s_level;
					if (__counter >= sustainTime)
						__amplitude = s_level;
				}
				else
					__amplitude = d_level;
				

				if (__amplitude <= 0.0001) {
					__amplitude = 0.0;
					__state = 5;
				}
				break;
			case 4://Release
				__amplitude = pow(-(static_cast<double>(__counter) - releaseTime) / r_k, r_c);
				if (__counter>= releaseTime)
				{
					__counter = 0;
					__state++;
				}
				if (!isfinite(__amplitude))
					__amplitude = 0;
				break;
			case 5:
				__amplitude = 0.0;
				break;
		}
		buffer[i] = __amplitude;
		__counter++;
	}
}

void EnvelopeGenerator::setVelocity(uint8 vel)
{
	__vel = vel;
}

void EnvelopeGenerator::setSustain(uint8 sust)
{
	__sustain = sust;
}



void EnvelopeGenerator::Reset(uint8 vel) {
	__state = 0;
	__vel = vel;
	__velMulti = vel / 127.0;
	__counter = 0;
}


double EnvelopeGenerator::calculateSlope(double begin,double end, double time) {
	if (time == 0)
		return 0;
	return (end - begin) / time;
}

void EnvelopeGenerator::recalculateParameters()
{
}

EnvelopeGenerator::EnvelopeGenerator(int ID,double sampleRate):
	IVSTParameters(ID),
	__sampleRate(sampleRate),

	__state(5),

	__amplitude(0),
	__vel(127),
	__velMulti(1.0),
	__counter(0)
{
	__a_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_TIME");
	__a_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_LEVEL");
	__a_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_CURVE");
	__h_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_HOLD_TIME");
	__d_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_TIME");
	__d_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_LEVEL");
	__d_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_CURVE");
	__r_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_TIME");
	__r_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_CURVE");

	__s_time = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_TIME");
	__s_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_LEVEL");
	__s_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_CURVE");
}

EnvelopeGenerator::~EnvelopeGenerator()
{
}

bool EnvelopeGenerator::isActive() const {
	return __state != 5;
}


void EnvelopeGenerator::RenderImage(int __ID, Image * image)
{
	ScopedPointer<Graphics> g = new Graphics(*image);
	g->setImageResamplingQuality(Graphics::ResamplingQuality::highResamplingQuality);
	g->setColour(Colour::fromRGB(36, 36, 36));
	g->fillAll();
	g->setColour(Colour::fromRGB(26, 105, 180));
	AudioParameterFloat * __a_time, *__a_level, *__a_curve, *__h_time, *__d_time, *__d_level, *__d_curve, *__r_time, *__r_curve, *__s_level, *__s_time, *__s_curve;
	__a_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_TIME");
	__a_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_LEVEL");
	__a_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_ATTACK_CURVE");
	__h_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_HOLD_TIME");
	__d_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_TIME");
	__d_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_LEVEL");
	__d_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_DECAY_CURVE");
	__r_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_TIME");
	__r_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_RELEASE_CURVE");

	__s_time  = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_TIME");
	__s_level = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_LEVEL");
	__s_curve = Global->paramHandler->Get<AudioParameterFloat>(__ID, "ENV_SUSTAIN_CURVE");
	float __s_time_s = *__s_time;
	double a_level = *__a_level;
	double a_c = *__a_curve;
	

	double d_level = *__d_level;
	if (d_level > a_level)
		d_level = a_level;
	double d_c = *__d_curve;
	

	double s_level = *__s_level;
	if (s_level > d_level)
		s_level = d_level;
	double s_c = *__s_curve;
	

	double r_c = *__r_curve;
	

	if (s_level >= d_level)
	{
		__s_time_s = static_cast<float>((*__a_time + *__h_time + *__d_time + *__r_time)*0.1);
	}
	int __state = 0;
	double __amplitude = 0.0;
	int __imageHeight = image->getHeight();
	int __counter = 0;
	int __sampleRate = static_cast<int>((image->getWidth())/(*__a_time+ *__h_time + *__d_time + *__r_time + __s_time_s));

	int attackTime = (int)(*__a_time * __sampleRate);
	int holdTime = (int)(*__h_time * __sampleRate);
	int decayTime = (int)(*__d_time * __sampleRate);
	int releaseTime = (int)(*__r_time * __sampleRate);
	int sustainTime = (int)(__s_time_s * __sampleRate);
	double a_k = (attackTime)*pow(a_level, -1 / a_c);
	double d_k = (decayTime)*pow(a_level - d_level, -1 / d_c);
	double s_k = (sustainTime)*pow(d_level - s_level, -1 / s_c);
	double r_k = (releaseTime)*pow(d_level, -1 / r_c);

	float lastx = 0,lasty = 0;
	for (int i = 2; i < image->getWidth()-3; i++)
	{
		switch (__state)
		{
		case 0://Attack
			__amplitude = pow(static_cast<double>(__counter) / a_k, a_c);
			if (__counter >= attackTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 1://Hold
			__amplitude = a_level;
			if (__counter >= holdTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 2://Decay
			__amplitude = pow(-(static_cast<double>(__counter) - decayTime) / d_k, d_c) + d_level;
			if (__counter >= decayTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 3://Sustain
			if (s_level != d_level)
			{
				__amplitude = pow(-(static_cast<double>(__counter) - sustainTime) / s_k, s_c) + s_level;
				if (__counter >= sustainTime)
					__amplitude = s_level;
			}
			else
				__amplitude = d_level;


			if (__counter > sustainTime) {
				__counter = 0;
				__state = 4;
			}
			break;
		case 4://Release
			__amplitude = pow(-(static_cast<double>(__counter) - releaseTime) / r_k, r_c);
			if (__counter >= releaseTime)
			{
				__counter = 0;
				__state++;
			}
			break;
		case 5:
			__amplitude = 0.0;
			break;
		}
		
		if (std::isfinite<float>(static_cast<float>(__amplitude))) {
			if (i > 2)
				g->drawLine(lastx, lasty, static_cast<float>(i), __imageHeight - static_cast<float>(__amplitude*(__imageHeight-16))-3, 3.0f);
			lastx = static_cast<float>(i);
			lasty = __imageHeight - static_cast<float>(__amplitude*(__imageHeight - 16))-3;
		}
		__counter++;
	}
	g->setColour(Colour::fromRGB(26, 26, 26));
	g->drawRect(0, 0, image->getWidth(), image->getHeight(), 3);
}

void EnvelopeGenerator::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK_LEVEL", "Attack level", 0.01f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK_TIME", "Attack time", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_ATTACK_CURVE", "Attack curve", 0.01f, 10.0f, 2.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_HOLD_TIME", "Hold time", 0.0f, 2.0f, 0.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY_LEVEL", "Decay level", 0.01f, 1.0f, 0.9f);
	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY_TIME", "Decay time", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_DECAY_CURVE", "Decay curve", 0.01f, 10.0f, 1.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN_LEVEL", "Sustain level", 0.0f, 1.0f, 1.0f);
	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN_TIME", "Sustain time", 0.01f, 10.0f, 2.0f);
	Global->paramHandler->RegisterFloat(ID, "ENV_SUSTAIN_CURVE", "Sustain curve", 0.01f, 10.0f, 1.0f);

	Global->paramHandler->RegisterFloat(ID, "ENV_RELEASE_TIME", "Release time", 0.0f, 2.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "ENV_RELEASE_CURVE", "Release curve", 0.01f, 10.0f, 1.0f);

}