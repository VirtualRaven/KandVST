/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "PluginProcessor.h"
#include "PluginGUI.h"
#include "IWavetable.h"
#include "PresetManager.h"
#include "Pipeline.h"
#include "Components/SettingsComponent.h"
#include "PipelineManager.h"
#include "ConvolutionReverb.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter();


//==============================================================================

PluginProcessor::PluginProcessor()
    : AudioProcessor (getBusesProperties()),
	__sampleRate(0.0),
	processorReady(false),
	__wavePool(4)
{
	lastPosInfo.resetToDefault();
	__pipManager.dp = nullptr;
	__pipManager.fp = nullptr;
	doublePrecision = true;
	Global = new GLOBAL(this);
	// -1 = master

	setParameters<int,	EnvelopeGenerator, 
						DelayEffect<double>, 
						DistEffect<double>,
						WavetableOsc,
						Pipeline<double>, 
						LFO,
						FilterHP<double>,
						FilterLP<double>,
						SettingsComponent,
						ConvolutionReverb<double>,
						AboutPageComponent,
						PipelineManager<double>,
						BitcrushEffect<double>
			>({
			{0,1,2,3},
			{-1},
			{0,1,2,3},
			{0,1,2,3},
			{ 0 },
			{0,1},
			{-1,0,1,2,3},
			{-1,0,1,2,3},
			{-1},
			{-1},
			{-1},
			{-1},
			{-1}
			}, Global);

	Global->presetManager->RefreshPresets();
	
	*(Global->paramHandler->Get<AudioParameterBool>(0, "OSC_MIX_EN")) = 1; //Enable default oscillator
}


PluginProcessor::~PluginProcessor()
{
	Global->log->Write("Destory\n");
	freePipelineManager();
	freeWavetable();
	delete Global;
	delete getActiveEditor();
}

void PluginProcessor::freePipelineManager() {
	processorReady = false;
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
	if (__pipManager.dp != nullptr)
		__pipManager.dp->Reset();
	if (__pipManager.fp != nullptr)
		__pipManager.fp->Reset();
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
	XmlElement xml("KandVSTPreset");
	Global->presetManager->SavePreset(&xml);

	copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation(const void * data, int sizeInBytes)
{
	//this needs rewrite
	Global->log->Write("Set state\n");
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	Global->presetManager->LoadPreset(xmlState);

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
	static int lastMaxSamples = 0;
	Global->log->Write("Prepare To play\n");

	//Have any of the values updated?
	if (__sampleRate != newSampleRate || 
		doublePrecision != isUsingDoublePrecision() ||
		maxSamplesPerBlock != lastMaxSamples
		) {
		
		populateWavetable(newSampleRate,__wavePool);
		keyboardState.reset();
		freePipelineManager();
		//Thread::launch([this, newSampleRate, maxSamplesPerBlock]() {
			while (!wavetableRdy());
			if ((doublePrecision = isUsingDoublePrecision()) == true)
				__pipManager.dp = new PipelineManager<double>(newSampleRate, maxSamplesPerBlock,Global);
			else
				__pipManager.fp = new PipelineManager<float>(newSampleRate, maxSamplesPerBlock,Global);
			processorReady = true;
		//});
		
		

		__sampleRate = newSampleRate;
		lastMaxSamples = maxSamplesPerBlock;
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
	if (buffer.getNumSamples() == 0)
		return;

	if (!processorReady)
		return;
    const int numSamples = buffer.getNumSamples();
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

	updateCurrentTimeInfoFromHost();
	getPipeline<FloatType>()->genSamples(buffer, midiMessages, lastPosInfo);


	//for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		// buffer.clear (i, 0, numSamples);
	
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
	Global->log->Write("createEditor\n");
    return new PluginGUI(*this,Global);
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
