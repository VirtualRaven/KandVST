#include "ConvolutionReverb.h"
#define FADE 0.3

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__conv()
{
	File resp = File(File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/resp.wav"));
	__conv.prepare({sampleRate, (uint32)4096*2, 2 });
	__conv.loadImpulseResponse(resp, true, false, 0);
}

template<typename T>
ConvolutionReverb<T>::~ConvolutionReverb()
{
}
template<typename T>
void ConvolutionReverb<T>::RegisterParameters(int ID)
{

}

template<typename T>
bool ConvolutionReverb<T>::RenderBlock(AudioBuffer<T>& buffer, int len, bool empty)
{
	
	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

