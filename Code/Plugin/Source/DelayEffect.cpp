#include "DelayEffect.h"
#define FADE 0.3

template<typename T>
DelayEffect<T>::DelayEffect(int ID,double sampleRate):
	IEffect(sampleRate),
	IVSTParameters(ID),
	__delayBuffer(2,static_cast<int>(sampleRate/4)), //1/6 sec echo ,
	__delayLen(__delayBuffer.getNumSamples()),
	__delayPos(0)
{
	__delayMultiplier = Global->paramHandler->Get<AudioParameterFloat>(ID, "EX_DELAYMULTI");

	for (int i = 0; i < __delayBuffer.getNumSamples(); i++)
	{
		__delayBuffer.setSample(0, i,0);
		__delayBuffer.setSample(1, i, 0);

	}
}

template<typename T>
DelayEffect<T>::~DelayEffect()
{
}
template<typename T>
void DelayEffect<T>::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "EX_DELAYMULTI", "Delay", 0.0f, 1.0f, 0.2f);
}

template<typename T>
bool DelayEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	float multi = *__delayMultiplier;
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

template class DelayEffect<double>;
template class DelayEffect<float>;

