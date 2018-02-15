#include "LFOsc.h"
#include "Log.h"
#include "Global.h"
#include "ParameterHandler.h"


LFO::LFO(int bpm, double sampleRate, int ID) :
	__isActive(false),
	__phase(0.0),
	__wavetable(tables[WAVE_TYPE::SAW]),
	__sampleRate(sampleRate)
{
	__amount = Global.paramHandler->Get<AudioParameterFloat>(ID, "LFO_AMOUNT");
	__ratio = Global.paramHandler->Get<AudioParameterInt>(ID, "LFO_RATIO");
	__isActive = Global.paramHandler->Get<AudioParameterBool>(ID, "LFO_EN");
	__bpm = Global.paramHandler->Get<AudioParameterInt>(ID, "LFO_BPM");
}
double LFO::calcRatio()
{
	if ((*__ratio) > 0) return (*__ratio);
	else
	{
		return (1.0 / (abs(*__ratio) + 2.0));
	};
}
void LFO::apply(double& tmp)
{
	if (!*__isActive) return;
	double freq = (*__bpm) * calcRatio() / 60.0;
	double samp = __wavetable->getSample(__phase, freq);
	tmp *= pow(2.0, samp*(*__amount) / 12.0);
	__phase += __wavetable->getLength() * freq / __sampleRate;
	
}
void LFO::RegisterParameters(int ID)
{
	Global.paramHandler->RegisterBool(ID, "LFO_EN", "LFO", 0);
	Global.paramHandler->RegisterInt(ID, "LFO_BPM", "BPM", 30, 300, 120);
	Global.paramHandler->RegisterInt(ID, "LFO_RATIO", "LFO Ratio", -8, 64, 1); //TEMP!!!
	Global.paramHandler->RegisterFloat(ID, "LFO_AMOUNT", "LFO amount", 0.0, 5.0, 0.5);
}

void LFO::setWavetable(int t)
{
	__wavetable = tables[toWAVE_TYPE(t)];
}
