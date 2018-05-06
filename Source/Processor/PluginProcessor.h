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

#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H



#include "../JuceLibraryCode/JuceHeader.h"
#include "PipelineManager.h"
#include "Global.h"
#include "TemplateHelper.h"
#include "ParameterHandler.h"
class PluginGUI;
class PluginProcessor  : public AudioProcessor, private Timer
{
public:
	PluginProcessor();
    ~PluginProcessor();
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (! isUsingDoublePrecision());
        process (buffer, midiMessages);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (isUsingDoublePrecision());
        process (buffer, midiMessages);
    }


    bool hasEditor() const override                                             { return true; }
    AudioProcessorEditor* createEditor() override;

    const String getName() const override                                       { return JucePlugin_Name; }

    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return false; }

    double getTailLengthSeconds() const override                                { return 0.0; }

  
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& name) override;

	bool isReady();

	// Inherited via AudioProcessor
	virtual void getStateInformation(juce::MemoryBlock & destData) override;
	virtual void setStateInformation(const void * data, int sizeInBytes) override;


    MidiKeyboardState keyboardState;

	AudioPlayHead::CurrentPositionInfo lastPosInfo;
	GLOBAL * Global;
private:
	void RegisterParameters(int ID, GLOBAL* global);
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages);

	union PipUnion {
		PipelineManager<double>* dp;
		PipelineManager<float>* fp;
	};

	template<typename T>  PipelineManager<T>* getPipeline();

	bool doublePrecision;

	void freePipelineManager();

	PipUnion __pipManager;
	
	int __currentPreset = 0;

	float __db;

    Synthesiser synth;
    static BusesProperties getBusesProperties();

	void updateCurrentTimeInfoFromHost();

	double __sampleRate;
	bool processorReady;

	bool supportsDoublePrecisionProcessing() const override {
		return true;
	}

	ThreadPool __wavePool;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)

	

		// Inherited via Timer
		virtual void timerCallback() override;

};

#endif // !PLUGIN_PROCESSOR_H