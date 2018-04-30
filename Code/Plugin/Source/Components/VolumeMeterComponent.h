#ifndef VOLUME_METER_COMPONENT_H
#define VOLUME_METER_COMPONENT_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "Swatch.h"
#include "PluginProcessor.h"

class VolumeMeterComponent : public Component, private Timer, private AudioProcessor {
public:
	VolumeMeterComponent(GLOBAL* global);
	~VolumeMeterComponent();

	void paint(Graphics& g) override;
	void resized() override;

private:
	GLOBAL * Global;
	Rectangle<int> __0, __1, __2, __3, __4, __5, __6;
	std::vector<Rectangle<int>> rects;
	void drawVolume(float);
	bool __meterDrawn;
	float __db;

	// Inherited from AudioProcessor
	void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

	static BusesProperties getBusesProperties();

	// Inherited from Timer
	void timerCallback() override;


	// Inherited via AudioProcessor
	virtual const String getName() const override;

	virtual void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;

	virtual void releaseResources() override;

	virtual double getTailLengthSeconds() const override;

	virtual bool acceptsMidi() const override;

	virtual bool producesMidi() const override;

	virtual AudioProcessorEditor * createEditor() override;

	virtual bool hasEditor() const override;

	virtual int getNumPrograms() override;

	virtual int getCurrentProgram() override;

	virtual void setCurrentProgram(int index) override;

	virtual const String getProgramName(int index) override;

	virtual void changeProgramName(int index, const String & newName) override;

	virtual void getStateInformation(juce::MemoryBlock & destData) override;

	virtual void setStateInformation(const void * data, int sizeInBytes) override;

};
#endif VOLUME_METER_COMPONENT_H