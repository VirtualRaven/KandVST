#include "DistEffect.h"
#include <stdlib.h>

template<typename T>
DistEffect<T>::DistEffect(int ID, double samplerate) : IEffect(samplerate)
{
	__threshold = Global->paramHandler->Get<AudioParameterFloat>(ID, "DIST_TRSH");
	__isActive = Global->paramHandler->Get<AudioParameterBool>(ID, "DIST_EN");
}

template<typename T>
DistEffect<T>::~DistEffect()
{
}

template<typename T>
void DistEffect<T>::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "DIST_TRSH", "Distortion threshold",0.0f,2.0f,2.0f);
	Global->paramHandler->RegisterBool(ID, "DIST_EN", "Distortion enable", false);
}

template<typename T>
bool DistEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) {
	if (empty || !(*__isActive)) return false;
	auto buff = buffer.getArrayOfWritePointers();
	for (int i = 0; i < len; i++) {
		double samp0 = buff[0][i];
		double samp1 = buff[1][i];
		int mul1 = samp0 < 0 ? -1 : 1;
		int mul2 = samp1 < 0 ? -1 : 1;
		double t = (*__threshold);
		buff[0][i] = std::min(abs(samp0),t) * mul1;
		buff[1][i] = std::min(abs(samp1),t) * mul2;
	}
	return true;
}
template<typename T>
void DistEffect<T>::ProccessCommand(MidiMessage message) {

}

template class DistEffect<double>;
template class DistEffect<float>;