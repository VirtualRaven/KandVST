#include "LFO.h"
#include "Log.h"
#include "Global.h"
#include "ParameterHandler.h"
#include <sstream>

LFO* lfos[LFO_COUNT] = {};
LFO::LFO(int maxSamples, int ID, double sampleRate) :
	__isActive(false),
	__wavetable(tables[WAVE_TYPE::SINE]),
	__samples(maxSamples*2),
	__phase(0.0),
	__ID(ID),
	__sampleRate(sampleRate),
	__activeCheck(false)
{
	__amount = Global->paramHandler->Get<AudioParameterFloat>(ID, "LFO_AMOUNT");
	__ratio = Global->paramHandler->Get<AudioParameterInt>(ID, "LFO_RATIO");
	__waveType = Global->paramHandler->Get<AudioParameterInt>(ID, "LFO_TYPE");
	__isActive = Global->paramHandler->Get<AudioParameterBool>(ID, "LFO_EN");
}
double LFO::calcRatio()
{
	if ((*__ratio) > 0) return (*__ratio);
	else
	{
		return (1.0 / (abs(*__ratio) + 2.0));
	};
}
void LFO::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterBool(ID, "LFO_EN", "LFO", 0);
	Global->paramHandler->RegisterInt(ID, "LFO_RATIO", "LFO Ratio", -2, 16, 1); //TEMP!!!
	Global->paramHandler->RegisterInt(ID, "LFO_TYPE", "LFO Wave type", 0, 3, 0);
	Global->paramHandler->RegisterFloat(ID, "LFO_AMOUNT", "LFO amount", 0.0, 5.0, 0.5);
}
double LFO::getSample(int idx)
{
	if (!__activeCheck) return 0; //The buffer may not be filled
	return __samples[idx] * (*__amount); //maybe not use __amount at all?
}


void LFO::generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo)
{
	if ((*__isActive)) __activeCheck = true;
	else {
		__activeCheck = false;
		return;
	}
	__wavetable = (tables[toWAVE_TYPE(*__waveType)]); 

	double freq = (posInfo.bpm)* calcRatio() / 60.0;

	double inc = __wavetable->getLength() * freq / __sampleRate;
	if (posInfo.ppqPosition > 5) Global->log->Write("BIGGER THAN 5!!!");
	if (posInfo.isPlaying) __phase = posInfo.ppqPosition * __wavetable->getLength() * calcRatio();

	for (int i = 0; i < numSamples; i++)
	{
		__samples[i] = __wavetable->getSample(__phase, freq);
		__phase += inc;
	}
}


