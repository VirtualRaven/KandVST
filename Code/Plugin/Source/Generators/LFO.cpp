#include "LFO.h"
#include "Log.h"
#include "Global.h"
#include "ParameterHandler.h"
#include <sstream>

LFO* lfos[LFO_COUNT] = {};
LFO::LFO(int maxSamples, int ID, double sampleRate) :
	__wavetable(tables[WAVE_TYPE::SINE]),
	__phase(0.0),
	__ID(ID),
	__sampleRate(sampleRate),
	__activeCheck(false),
	__nrOfSamples(maxSamples)
{
	__samples = new double[maxSamples];
	__amount	 = Global->paramHandler->Get<AudioParameterFloat>(ID, "LFO_AMOUNT");
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
	Global->paramHandler->RegisterInt(ID, "LFO_RATIO", "LFO Ratio", -6, 16, 1); //TEMP!!!
	Global->paramHandler->RegisterInt(ID, "LFO_TYPE", "LFO Wave type", 0, 3, 0);
	Global->paramHandler->RegisterFloat(ID, "LFO_AMOUNT", "LFO amount", 0.0, 1.0, 0.5);
}

double* LFO::getPointer()
{
	if (!__activeCheck) return nullptr;
	return __samples;
}
float LFO::getAmount()
{
	if (!__activeCheck) return 0.0f;
	return (*__amount);
}
bool LFO::isActive() {
	return __activeCheck;
}
void LFO::generate(int numSamples, AudioPlayHead::CurrentPositionInfo& posInfo)
{
	if (numSamples > __nrOfSamples) {
		delete[] __samples;
		__nrOfSamples = numSamples;
		__samples = new double[numSamples];
	}

	if (!(*__isActive)) {
		if (!posInfo.isPlaying) {
			__phase = fmod(__phase + (IWavetable::getLength() * ((posInfo.bpm)* calcRatio() / 60.0) / __sampleRate) * numSamples, IWavetable::getLength());
		}
		__activeCheck = false;
		return;
	}
	//WAVE_TYPE type = toWAVE_TYPE(*__waveType);
	
	double freq = (posInfo.bpm)* calcRatio() / 60.0;

	double inc = IWavetable::getLength() * freq / __sampleRate;

	static int nrLoopsPlayed = 0;
	static double prevPos = posInfo.ppqPosition;

	if (posInfo.isPlaying) {
		if (posInfo.isLooping) {

			if (prevPos > posInfo.ppqPosition) {
				nrLoopsPlayed++;
			}
			__phase = fmod((nrLoopsPlayed * posInfo.timeSigNumerator + posInfo.ppqPosition) * IWavetable::getLength() * calcRatio(), IWavetable::getLength());
		} else 
			__phase = fmod(posInfo.ppqPosition * IWavetable::getLength() * calcRatio(), IWavetable::getLength());
		prevPos = posInfo.ppqPosition;
	}
	else {
		nrLoopsPlayed = 0;
		prevPos = 0;
	}
		
		// TMPTMPTMP
	switch (toWAVE_TYPE(*__waveType)) {
	case SINE	:
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<SINE>(tgt);
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	case SQUARE	: 
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<SQUARE>(tgt);
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	case TRI	: 
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<TRI>(tgt);
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	case SAW	: 
		for (int i = 0; i < numSamples; i++) {
			auto tgt = IWavetable::getLoc(__phase, freq);
			__samples[i] = getSampleFromLoc<SAW>(tgt);
			__phase = fmod(__phase + inc, IWavetable::getLength());
		}
		break;
	}

	__activeCheck = true;
}




