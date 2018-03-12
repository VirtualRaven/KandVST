#include "LFO.h"
#include "Log.h"
#include "Global.h"
#include "ParameterHandler.h"
#include <sstream>

LFO* lfos[LFO_COUNT] = {};
LFO::LFO(int maxSamples, int ID, double sampleRate) :
	__isActive(false),
	__wavetable(tables[WAVE_TYPE::SINE]),
	__phase(0.0),
	__ID(ID),
	__sampleRate(sampleRate),
	__activeCheck(false),
	__nrOfSamples(maxSamples)
{
	__samples = new double[maxSamples];
	__amount	 = Global->paramHandler->Get<AudioParameterFloat>(ID, "LFO_AMOUNT");
	__freqamount = Global->paramHandler->Get<AudioParameterFloat>(ID, "LFO_FREQ_AMOUNT");
	__ratio		 = Global->paramHandler->Get<AudioParameterInt>(ID, "LFO_RATIO");
	__waveType	 = Global->paramHandler->Get<AudioParameterInt>(ID, "LFO_TYPE");
	__isActive	 = Global->paramHandler->Get<AudioParameterBool>(ID, "LFO_EN");
}
LFO::~LFO()
{
	delete[] __samples;
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
	Global->paramHandler->RegisterFloat(ID, "LFO_AMOUNT", "LFO amount", 0.0, 1.0, 0.5);
	Global->paramHandler->RegisterFloat(ID, "LFO_FREQ_AMOUNT", "LFO Frequency Amount",0.0,24.0,2.0);
}

double* LFO::getPointer()
{
	if (!__activeCheck) return nullptr;
	return __samples;
}
float LFO::getFreqAmount()
{
	return (*__freqamount);
}
float LFO::getAmount()
{
	return (*__amount);
}

void LFO::generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo)
{
	if (numSamples > __nrOfSamples) {
		delete[] __samples;
		__nrOfSamples = numSamples;
		__samples = new double[numSamples];
		Global->log->Write("New lfo buffer created\n");
	}
	if (!(*__isActive)) {
		if (!posInfo.isPlaying) {
			__phase = fmod(__phase + (IWavetable::getLength() * ((posInfo.bpm)* calcRatio() / 60.0) / __sampleRate) * numSamples, IWavetable::getLength());
		}
		__activeCheck = false;
		return;
	}
	if (__ID == 1) {
		double test = 0;
		bool bla = __phase < 0;
	}
	//WAVE_TYPE type = toWAVE_TYPE(*__waveType);
	
	double freq = (posInfo.bpm)* calcRatio() / 60.0;

	double inc = __wavetable->getLength() * freq / __sampleRate;

	if (posInfo.isPlaying) __phase = fmod(posInfo.ppqPosition * IWavetable::getLength() * calcRatio(), IWavetable::getLength());

	for (int i = 0; i < numSamples; i++)
	{
		auto tgt = IWavetable::getLoc(__phase, freq);
		//__samples[i] = getSampleFromLoc<SQUARE>(tgt);
		// TMPTMPTMP
		switch (toWAVE_TYPE(*__waveType)) {
		case SAW	: 
			__samples[i] = getSampleFromLoc<SAW>(tgt);
			break;
		case SQUARE	: 
			__samples[i] = getSampleFromLoc<SQUARE>(tgt);
			break;
		case TRI	: 
			__samples[i] = getSampleFromLoc<TRI>(tgt);
			break;
		case SINE	: 
			__samples[i] = getSampleFromLoc<SINE>(tgt);
			break;
		}
		__phase += inc;
	}
	__phase = fmod(__phase, IWavetable::getLength());
	__activeCheck = true;
}




