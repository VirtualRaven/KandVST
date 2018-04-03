#include "DelayEffect.h"
#define FADE 0.3

template<typename T>
DelayEffect<T>::DelayEffect(int ID, double sampleRate,GLOBAL*global) :
	IEffect<T>(sampleRate),
	IVSTParameters(ID),
	__delayBuffer(2, static_cast<int>(sampleRate * 4)),	// Buffer size is max delay
	__delayLen(0),	// Actual delay length
	__prevDelayLen(0),
	__delayPos(0)
{
	Global = global;
	__isEnabled = global->paramHandler->Get<AudioParameterBool>(ID, "DELAY_EN");
	__delayMultiplier = Global->paramHandler->Get<AudioParameterFloat>(ID, "EX_DELAYMULTI");
	__delayLenMult = Global->paramHandler->Get<AudioParameterFloat>(ID, "EX_DELAYLENGTH");

	for (int i = 0; i < __delayBuffer.getNumSamples(); i++)
	{
		__delayBuffer.setSample(0, i, 0);
		__delayBuffer.setSample(1, i, 0);
	}
}

template<typename T>
DelayEffect<T>::~DelayEffect()
{
}
template<typename T>
void DelayEffect<T>::RegisterParameters(int ID,GLOBAL*Global)
{
	Global->paramHandler->RegisterBool(ID, "DELAY_EN", "DELAY", 0);
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYMULTI", "Delay", 0.0f, 1.0f, 0.2f);
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYLENGTH", "Delay Length", 0.125f, 4.0f, 0.25f);
}

template<typename T>
bool DelayEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	// Check if enabled
	if (*__isEnabled == false || empty)
		return false;

	float multi =*__delayMultiplier;
	float lenmult =  *__delayLenMult;

	__delayLen = (this->__sampleRate * lenmult);

	if (__delayLen != __prevDelayLen)
	{
		// Clear buffer when the length has changed
		__delayBuffer.clear();
	}
	__prevDelayLen = __delayLen;

	if (empty) {
		if(__delayBuffer.getMagnitude(0,__delayLen) < 0.01 )
			return false;
		else {
			auto buff = buffer.getArrayOfWritePointers();
			auto dbuff = __delayBuffer.getArrayOfWritePointers();
			for (int i = 0; i < len; i++)
			{
				dbuff[0][__delayPos] *=multi ;
				dbuff[1][__delayPos] *=multi ;

				__delayPos = (__delayPos + 1) % __delayLen;

				buff[0][i] += dbuff[0][__delayPos] * multi;
				buff[1][i] += dbuff[1][__delayPos] * multi;
			}
			return true;
		}
	}
	else{
	
	
		auto buff = buffer.getArrayOfWritePointers();
		auto dbuff = __delayBuffer.getArrayOfWritePointers();
		for (int i = 0; i < len; i++)
		{
			dbuff[0][__delayPos] = dbuff[0][__delayPos] * multi + buff[0][i];
			dbuff[1][__delayPos] = dbuff[1][__delayPos] * multi + buff[1][i];

			__delayPos = (__delayPos + 1) % __delayLen;
			
			buff[0][i] += dbuff[0][__delayPos] * multi;
			buff[1][i] += dbuff[1][__delayPos] * multi;
		}

		return true;
	}
}

template<typename T>
void DelayEffect<T>::ProccessCommand(MidiMessage message)
{
}

template<typename T>
void DelayEffect<T>::Reset()
{
	__delayBuffer.clear();
}

template class DelayEffect<double>;
template class DelayEffect<float>;

