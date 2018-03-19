#include "ConvolutionReverb.h"
#include "juce_dsp\juce_dsp.h"
#define FADE 0.3

template<typename T>
ConvolutionReverb<T>::ConvolutionReverb(int ID, double sampleRate, int maxBuffHint) :
	IEffect(sampleRate),
	IVSTParameters(ID),
	__conv(),
	__maxBuffHint(maxBuffHint),
	__responseBuffer()
{


	// Read response
	File resp = File(File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() + String("/KandVST/resp.wav"));

	AudioFormatManager manager;
	manager.registerBasicFormats();
	ScopedPointer<AudioFormatReader> reader = manager.createReaderFor(resp);

	AudioSampleBuffer buffer = AudioSampleBuffer();
	buffer.setSize(2, reader->lengthInSamples, false, false, false);
	reader->read(&buffer, 0, 400, 0, true, true);

	// Convert to <T>
	__responseBuffer.makeCopyOf(buffer);
	__responseBufferLen = reader->lengthInSamples;
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
	for (int i = 0; i < len; i++)
	{
		for (int h = 0; h < __responseBufferLen; h++)
		{
			
			T currentLeft = 0;
			T currentRight = 0;

			if (i - h > 0)
			{
				currentLeft = buffer.getSample(0, i - h);
				currentRight = buffer.getSample(1, i - h);
			}

			buffer.setSample(0, i, currentLeft * __responseBuffer.getSample(0, h));
			buffer.setSample(1, i, currentRight * __responseBuffer.getSample(1, h));
		}
	}

	return true;
}

template<typename T>
void ConvolutionReverb<T>::ProccessCommand(MidiMessage message)
{
}

template class ConvolutionReverb<double>;
template class ConvolutionReverb<float>;

