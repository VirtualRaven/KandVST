#include "VolumeMeterComponent.h"

VolumeMeterComponent::~VolumeMeterComponent()
{
	stopTimer();
}
VolumeMeterComponent::VolumeMeterComponent(GLOBAL * global)
	: AudioProcessor(getBusesProperties())
{
	Global = global;
	rects.reserve(8);

	rects.push_back(__0 = Rectangle<int>(50, 25));
	rects.push_back(__1 = Rectangle<int>(__0));
	rects.push_back(__2 = Rectangle<int>(__0));
	rects.push_back(__3 = Rectangle<int>(__0));
	rects.push_back(__4 = Rectangle<int>(__0));
	rects.push_back(__5 = Rectangle<int>(__0));
	rects.push_back(__6 = Rectangle<int>(__0));
	
	__meterDrawn = false;
	__db = 0.f;

}

void VolumeMeterComponent::paint(Graphics & g) 
{
		int y = 185;
		int x = getLocalBounds().reduced(5).getX();
		int i = 0;
		for(Rectangle<int> rec :  rects) {
			rec.setX(x);
			rec.setY(y);

			g.setColour(Swatch::white);
			g.drawRect(rec, 2);

			
			g.setColour(Swatch::accentOrange.brighter(0.4f).withSaturation(1.2f));
			g.fillRect(rec);

			g.setColour(Swatch::white);
			g.drawText(std::to_string(__db), rec, Justification::centred, false);
			y -= 30;
			i++;
		}
		__meterDrawn = true;
	}

void VolumeMeterComponent::resized() {
	
}

void VolumeMeterComponent::processBlock(AudioSampleBuffer & buffer, MidiBuffer &)
{
	
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();

	//for (int i = totalNumInputChannels; i < totalNumOutputChannels; i++) 
	//	buffer.clear(i, 0, buffer.getNumSamples());


	for (int channel = 0; channel < totalNumInputChannels; ++channel) {

		float* channelData = buffer.getWritePointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
			__db = buffer.getSample(channel, sample);
			repaint();
		}
	}
	
}


void VolumeMeterComponent::drawVolume(float db) {
	Graphics g;


}

AudioProcessor::BusesProperties VolumeMeterComponent::getBusesProperties()
{
	return BusesProperties().withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true);
}
