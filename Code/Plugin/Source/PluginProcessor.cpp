#include "PluginProcessor.h"
#include "PluginGUI.h"
#include "IWavetable.h"
#include "OscillatorMixer.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter();

GLOBAL * Global;
//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor (getBusesProperties())
{
	__pipManager = nullptr;
	Global = new GLOBAL();
	Global->paramHandler=  new ParameterHandler(*this);
	Global->log = new Log("log.txt");

	setParameters<int, EnvelopeGenerator, ExampleEffect, WavetableOsc, OscillatorMixer>({ {0,1,2,3},{0},{0,1,2,3},{ 0 } });


	*(Global->paramHandler->Get<AudioParameterBool>(0, "OSC_MIX_EN")) = 1; //Enable default oscillator

}


PluginProcessor::~PluginProcessor()
{
	delete __pipManager;
	freeWavetable();
	delete Global;
}

void PluginProcessor::reset()
{
	Global->log->Write("Reset\n");
}



//==============================================================================
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only mono/stereo and input/output must have same layout
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
    const AudioChannelSet& mainInput  = layouts.getMainInputChannelSet();

    // input and output layout must either be the same or the input must be disabled altogether
    if (! mainInput.isDisabled() && mainInput != mainOutput)
        return false;

    // do not allow disabling the main buses
    if (mainOutput.isDisabled())
        return false;

    // only allow stereo and mono
    if (mainOutput.size() > 2)
        return false;

    return true;
}

AudioProcessor::BusesProperties PluginProcessor::getBusesProperties()
{
    return BusesProperties().withInput  ("Input",  AudioChannelSet::stereo(), true)
                            .withOutput ("Output", AudioChannelSet::stereo(), true);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock & destData)
{
	//this needs rewrite
	XmlElement xml("MYPLUGINSETTINGS");

	for (auto* param : getParameters())
		if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
			xml.setAttribute(String("_") + p->paramID, p->getValue());

	copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation(const void * data, int sizeInBytes)
{
	//this needs rewrite

	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState != nullptr)
	{
		if (xmlState->hasTagName("MYPLUGINSETTINGS"))
		{
			for (auto* param : getParameters())
				if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
					p->setValue((float)xmlState->getDoubleAttribute(String("_") + p->paramID, p->getValue()));
		}
	}

}

void PluginProcessor::prepareToPlay (double newSampleRate, int maxSamplesPerBlock)
{
	populateWavetable(newSampleRate);

    keyboardState.reset();
	__pipManager = new PipelineManager(newSampleRate, maxSamplesPerBlock);


}

void PluginProcessor::releaseResources()
{
    keyboardState.reset();

}

template <typename FloatType>
void PluginProcessor::process (AudioBuffer<FloatType>& buffer,
                                            MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();

    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
	__pipManager->genSamples(buffer, midiMessages);

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginGUI (*this);
}
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
