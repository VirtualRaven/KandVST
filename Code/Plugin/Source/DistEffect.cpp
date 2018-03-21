#include "DistEffect.h"
#include <stdlib.h>

template<typename T>
DistEffect<T>::DistEffect(double samplerate) : IEffect(samplerate){}

template<typename T>
DistEffect<T>::~DistEffect()
{
}

template<typename T>
bool DistEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) {
	if (empty) return false;
	auto buff = buffer.getArrayOfWritePointers();
	for (int i = 0; i < len; i++) {
		double samp0 = buff[0][i];
		double samp1 = buff[1][i];
		int mul = samp0 < 0 ? -1 : 1;
		buff[0][i] = std::min(abs(samp0), 0.01) * mul;
		buff[1][i] = std::min(abs(samp1), 0.01) * mul;
	}
	return true;
}
template<typename T>
void DistEffect<T>::ProccessCommand(MidiMessage message) {

}

template class DistEffect<double>;
template class DistEffect<float>;