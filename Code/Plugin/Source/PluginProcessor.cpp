#include "PluginProcessor.h"
#include "PluginGUI.h"
#include "IWavetable.h"
#include "PresetManager.h"
#include "Pipeline.h"
#include "Components\SettingsComponent.h"
#include "PipelineManager.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter();

GLOBAL * Global;
//==============================================================================

PluginProcessor::PluginProcessor()
    : AudioProcessor (getBusesProperties()),
	__sampleRate(0.0),
	processorReady(false)
{
	lastPosInfo.resetToDefault();
	__gui = nullptr;
	__pipManager.dp = nullptr;
	__pipManager.fp = nullptr;
	doublePrecision = true;
	Global = new GLOBAL();
	Global->paramHandler =  new ParameterHandler(*this);
	Global->log = new Log("log.txt");
	// -1 = master

	setParameters<int,	EnvelopeGenerator, 
						DelayEffect<double>, 
						WavetableOsc,
						Pipeline<double>, 
						LFO,
						FilterHP<double>,
						FilterLP<double>,
						SettingsComponent,
						PipelineManager<double>
			>({
			{0,1,2,3},
			{0},
			{0,1,2,3},
			{ 0 },
			{0,1,2,3},
			{-1,0,1,2,3},
			{-1,0,1,2,3},
			{-1},
			{-1} 
			});


	Global->presetManager = new PresetManager(this);
	Global->presetManager->RefreshPresets();
	
	//*(Global->paramHandler->Get<AudioParameterBool>(0, "OSC_MIX_EN")) = 1; //Enable default oscillator
	__gui = new PluginGUI(*this);
}


PluginProcessor::~PluginProcessor()
{
	Global->log->Write("Destory\n");
	//freePipelineManager();
	//freeWavetable();
	//delete Global;
}

void PluginProcessor::freePipelineManager() {
	if (doublePrecision) {
		delete __pipManager.dp;
		__pipManager.dp = nullptr;
	}
	else {
		delete __pipManager.fp;
		__pipManager.fp = nullptr;
	}

	
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

void PluginProcessor::timerCallback()
{
}

int PluginProcessor::getNumPrograms()
{
	return Global->presetManager->GetPresetCount();
}

int PluginProcessor::getCurrentProgram()
{
	return Global->presetManager->GetCurrentPreset();
}

void PluginProcessor::setCurrentProgram(int index)
{
	Global->presetManager->LoadPreset(index);
}

const String PluginProcessor::getProgramName(int index)
{
	return Global->presetManager->GetPresetNames().at(index);
}

void PluginProcessor::changeProgramName(int , const String & )
{
	//TODO
}

bool PluginProcessor::isReady()
{
	return processorReady;
}

void PluginProcessor::getStateInformation(juce::MemoryBlock & destData)
{
	Global->log->Write("Get state\n");
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
	Global->log->Write("Set state\n");
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

template<>  PipelineManager<double>* PluginProcessor::getPipeline<double>() {
	if (this->doublePrecision != true)
		throw std::invalid_argument("Plugin in single precision mode, but host asked for double precision ");
	return this->__pipManager.dp;
}

template<> PipelineManager<float>* PluginProcessor::getPipeline<float>() {
	if (this->doublePrecision != false)
		throw std::invalid_argument("Plugin in double precision mode, but host asked for single precision ");
	return this->__pipManager.fp;
}

void PluginProcessor::prepareToPlay (double newSampleRate, int maxSamplesPerBlock)
{
	Global->log->Write("Prepare To play\n");

	if (__sampleRate != newSampleRate) {
		populateWavetable(newSampleRate);
		keyboardState.reset();
		freePipelineManager();
		Thread::launch([this, newSampleRate, maxSamplesPerBlock]() {
			while (!wavetableRdy());
			if ((doublePrecision = isUsingDoublePrecision()) == true)
				__pipManager.dp = new PipelineManager<double>(newSampleRate, maxSamplesPerBlock);
			else
				__pipManager.fp = new PipelineManager<float>(newSampleRate, maxSamplesPerBlock);

			__gui->InitializeGui();
			processorReady = true;
		});
		
		

		__sampleRate = newSampleRate;
	}
}

void PluginProcessor::releaseResources()
{
    keyboardState.reset();

}

template <typename FloatType>
void PluginProcessor::process (AudioBuffer<FloatType>& buffer,
                                            MidiBuffer& midiMessages)
{
	if (!processorReady)
		return;
    const int numSamples = buffer.getNumSamples();
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

	getPipeline<FloatType>()->genSamples(buffer, midiMessages, lastPosInfo);


    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);

	updateCurrentTimeInfoFromHost();
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
	Global->log->Write("createEditor\n");
	__gui = new PluginGUI(*this);
    return __gui;
}
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

void PluginProcessor::updateCurrentTimeInfoFromHost()
{
	if (AudioPlayHead* ph = getPlayHead())
	{
		AudioPlayHead::CurrentPositionInfo newTime;

		if (ph->getCurrentPosition(newTime))
		{
			lastPosInfo = newTime;  // Successfully got the current time from the host..
			return;
		}
	}

	// If the host fails to provide the current time, we'll just reset our copy to a default..
	lastPosInfo.resetToDefault();
}
