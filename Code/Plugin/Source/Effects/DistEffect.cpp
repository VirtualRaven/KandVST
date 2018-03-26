#include "DistEffect.h"
#include "DistEffect.h"
#include "LFO.h"
#include <stdlib.h>

template<typename T>
DistEffect<T>::DistEffect(int ID, double samplerate) : IEffect(samplerate)
{
	__threshold = Global->paramHandler->Get<AudioParameterFloat>(ID, "DIST_TRSH");
	__isActive = Global->paramHandler->Get<AudioParameterBool>(ID, "DIST_EN");
	__lfoIndex = Global->paramHandler->Get<AudioParameterInt>(ID, "DIST_LFO");
}

template<typename T>
DistEffect<T>::~DistEffect()
{
}

template<typename T>
void DistEffect<T>::RegisterParameters(int ID)
{
	Global->paramHandler->RegisterFloat(ID, "DIST_TRSH", "Distortion threshold",0.0f,1.0f,1.0f);
	Global->paramHandler->RegisterBool(ID, "DIST_EN", "Distortion enable", false);
	Global->paramHandler->RegisterInt(ID, "DIST_LFO", "Dist lfo", 0, 2, 0);
}

template<typename T>
bool DistEffect<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty) {
	if (empty || !(*__isActive)) return false;
	double t = (*__threshold);
	auto buff = buffer.getArrayOfWritePointers();
	if ((*__lfoIndex) > 0) {
		double amount = lfos[(*__lfoIndex)-1]->getAmount();
		if (amount > 0.0) {
			t -= amount * t * ((lfos[(*__lfoIndex) - 1]->getPointer()[0] + 1.0) / 2.0);
		}
	}

	for (int i = 0; i < len; i++) {
		double samp0 = buff[0][i];
		double samp1 = buff[1][i];
		int mul1 = samp0 < 0 ? -1 : 1;
		int mul2 = samp1 < 0 ? -1 : 1;
		buff[0][i] = std::min(abs(samp0),1.1-t) * mul1;
		buff[1][i] = std::min(abs(samp1),1.1-t) * mul2;
	}
	return true;
}
template<typename T>
void DistEffect<T>::ProccessCommand(MidiMessage message) {

}

template<typename T>
void DistEffect<T>::Reset()
{
}

template class DistEffect<double>;
template class DistEffect<float>;