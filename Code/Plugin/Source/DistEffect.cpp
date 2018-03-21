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
		double x = buff[0][i];
		double samp = x*3.0/2.0 * (1.0 - pow(x,2)/2.0);
		buff[0][i] = std::min(x, 0.01);
		buff[1][i] = std::min(x, 0.01);
	}
	return true;
}
template<typename T>
void DistEffect<T>::ProccessCommand(MidiMessage message) {

}

template class DistEffect<double>;
template class DistEffect<float>;