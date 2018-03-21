#include "DistEffect.h"
#include <stdlib.h>

template<typename T>
DistEffect<T>::DistEffect(int ID, double samplerate) : IEffect(samplerate)
{
	__threshold = Global->paramHandler->Get<AudioParameterFloat>(ID, "DIST_TRSH");
}

template<typename T>
DistEffect<T>::~DistEffect()
{
}

template<typename T>
void DistEffect<T>::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "DIST_TRSH", "Distortion",0.0f,3.0f,3.0f);
}

template<typename T>
bool DistEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) {
	if (empty) return false;
	auto buff = buffer.getArrayOfWritePointers();
	for (int i = 0; i < len; i++) {
		double samp0 = buff[0][i];
		double samp1 = buff[1][i];
		int mul = samp0 < 0 ? -1 : 1;
		double t = (*__threshold);
		buff[0][i] = std::min(abs(samp0),t) * mul;
		buff[1][i] = std::min(abs(samp1),t) * mul;
	}
	return true;
}
template<typename T>
void DistEffect<T>::ProccessCommand(MidiMessage message) {

}

template class DistEffect<double>;
template class DistEffect<float>;