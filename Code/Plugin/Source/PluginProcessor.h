#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H



#include "../JuceLibraryCode/JuceHeader.h"
#include "PipelineManager.h"
#include "Global.h"
#include "TemplateHelper.h"
#include "OscillatorMixer.h"
#include "ParameterHandler.h"

class PluginProcessor  : public AudioProcessor
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

  
    int getNumPrograms() override                                               { return 0; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        { return String(); }
    void changeProgramName (int /*index*/, const String& /*name*/) override     {}

	// Inherited via AudioProcessor
	virtual void getStateInformation(juce::MemoryBlock & destData) override;
	virtual void setStateInformation(const void * data, int sizeInBytes) override;

    MidiKeyboardState keyboardState;
private:
    template <typename FloatType>
    void process (AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages);

	PipelineManager* __pipManager;

    Synthesiser synth;
    static BusesProperties getBusesProperties();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)

	
};

#endif // !PLUGIN_PROCESSOR_H