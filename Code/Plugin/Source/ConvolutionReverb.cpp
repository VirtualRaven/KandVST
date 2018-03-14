#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"
#define FADE 0.3

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__conv(),
	__maxBuffHint(maxBuffHint)
{
	File resp = File(File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/resp.wav"));
	__conv.prepare({sampleRate, (uint32) maxBuffHint, 2 });

	auto maxSize = static_cast<T> (roundDoubleToInt(8192.0 *  sampleRate / 44100.0));
	__conv.loadImpulseResponse(resp, true, true, maxSize * 10);
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
	AudioBuffer<float> b;
	b.makeCopyOf(buffer, true);
	auto cont = dsp::ProcessContextReplacing<float>(juce::dsp::AudioBlock<float>(b));
	__conv.process(cont);
	buffer.makeCopyOf(b, true);
	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

