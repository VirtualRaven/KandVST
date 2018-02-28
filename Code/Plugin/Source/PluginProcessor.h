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

	// Inherited via AudioProcessor
	virtual void getStateInformation(juce::MemoryBlock & destData) override;
	virtual void setStateInformation(const void * data, int sizeInBytes) override;

    MidiKeyboardState keyboardState;
private:
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages);

	PipelineManager* __pipManager;
	int __currentPreset = 0;

    Synthesiser synth;
    static BusesProperties getBusesProperties();

	double __sampleRate;
	PluginGUI * __gui;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)

	

		// Inherited via Timer
		virtual void timerCallback() override;

};

#endif // !PLUGIN_PROCESSOR_H