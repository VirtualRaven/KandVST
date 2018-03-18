#include "MixerComponent.h"

MixerComponent::~MixerComponent()
{
}


MixerComponent::MixerComponent()
{
	__osc1 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(0, "OSC_MIX_AMP"));
	__osc2 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(1, "OSC_MIX_AMP"));
	__osc3 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(2, "OSC_MIX_AMP"));
	__osc4 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(3, "OSC_MIX_AMP"));
	__masterMixer = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(-1, "MASTER_GAIN"));

	__masterPan = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(-1, "MASTER_GAIN"));
	__pan1 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(0, "OSC_PAN"));
	__pan2 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(1, "OSC_PAN"));
	__pan3 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(2, "OSC_PAN"));
	__pan4 = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(3, "OSC_PAN"));


	//===========================
	// ADD AND MAKE VISIBLE

	addAndMakeVisible(__osc1);
	addAndMakeVisible(__osc2);
	addAndMakeVisible(__osc3);
	addAndMakeVisible(__osc4);
	addAndMakeVisible(__masterMixer);

	addAndMakeVisible(__masterPan);
	addAndMakeVisible(__panR);
	addAndMakeVisible(__panL);
	addAndMakeVisible(__pan1);
	addAndMakeVisible(__pan1L);
	addAndMakeVisible(__pan1R);
	addAndMakeVisible(__pan2);
	addAndMakeVisible(__pan2L);
	addAndMakeVisible(__pan2R);
	addAndMakeVisible(__pan3);
	addAndMakeVisible(__pan3L);
	addAndMakeVisible(__pan3R);
	addAndMakeVisible(__pan4);
	addAndMakeVisible(__pan4L);
	addAndMakeVisible(__pan4R);


	//==========================

	__osc1->setSliderStyle(Slider::LinearVertical);
	__osc1->setTextBoxStyle(__osc1->NoTextBox, true, 55, 15);
	__osc1Label.setText("OSC 1", NotificationType::dontSendNotification);
	__osc1Label.attachToComponent(__osc1, false);
	__osc1Label.setJustificationType(Justification::centred);

	__osc2->setSliderStyle(Slider::LinearVertical);
	__osc2->setTextBoxStyle(__osc2->NoTextBox, true, 55, 15);
	__osc2Label.setText("OSC 2", NotificationType::dontSendNotification);
	__osc2Label.attachToComponent(__osc2, false);
	__osc2Label.setJustificationType(Justification::centred);


	__osc3->setSliderStyle(Slider::LinearVertical);
	__osc3->setTextBoxStyle(__osc3->NoTextBox, true, 55, 15);
	__osc3Label.setText("OSC 3", NotificationType::dontSendNotification);
	__osc3Label.attachToComponent(__osc3, false);
	__osc3Label.setJustificationType(Justification::centred);

	__osc4->setSliderStyle(Slider::LinearVertical);
	__osc4->setTextBoxStyle(__osc4->NoTextBox, true, 55, 15);
	__osc4Label.setText("OSC 4", NotificationType::dontSendNotification);
	__osc4Label.attachToComponent(__osc4, false);
	__osc4Label.setJustificationType(Justification::centred);

	__masterMixer->setSliderStyle(Slider::LinearVertical);
	__masterMixer->setTextBoxStyle(__masterMixer->NoTextBox, true, 55, 15);
	__masterLabel.setText("MASTER", NotificationType::dontSendNotification);
	__masterLabel.attachToComponent(__masterMixer, false);
	__masterLabel.setJustificationType(Justification::centred);

	//====================================
	//Pans
	__masterPan->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__masterPan->setTextBoxStyle(__masterPan->NoTextBox, true, 10, 10);

	__pan1->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pan1->setTextBoxStyle(__pan1->NoTextBox, true, 10, 10);

	__pan2->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pan2->setTextBoxStyle(__pan2->NoTextBox, true, 10, 10);

	__pan3->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pan3->setTextBoxStyle(__pan3->NoTextBox, true, 10, 10);

	__pan4->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__pan4->setTextBoxStyle(__pan4->NoTextBox, true, 10, 10);
	//====================================
	// Pan labels
	__panL.setText("L", NotificationType::dontSendNotification);
	__panR.setText("R", NotificationType::dontSendNotification);

	__pan1L.setText("L", NotificationType::dontSendNotification);
	__pan1R.setText("R", NotificationType::dontSendNotification);
	
	__pan2L.setText("L", NotificationType::dontSendNotification);
	__pan2R.setText("R", NotificationType::dontSendNotification);
	
	__pan3L.setText("L", NotificationType::dontSendNotification);
	__pan3R.setText("R", NotificationType::dontSendNotification);
	
	__pan4L.setText("L", NotificationType::dontSendNotification);
	__pan4R.setText("R", NotificationType::dontSendNotification);
	//=====================================

}

void MixerComponent::paint(Graphics& g) {

	int width = __bounds.getWidth();
	int height = __bounds.getHeight();
	int fontHeight = __bounds.getHeight() * 0.1;
	int fontSize = __bounds.getHeight() * 0.08;

	g.setColour(Colour::fromRGB(60, 60, 60));
	g.drawRect(Rectangle<int>(10, 10, width, height), 2.0f);
	g.fillRect(Rectangle<int>(10, 10, width, fontHeight));

	Font mixer(fontSize, Font::FontStyleFlags::bold);
	g.setColour(Colours::white);
	g.setFont(mixer);
	g.drawText("MIXER", Rectangle<int>(10, 10, width, fontHeight), Justification::centred, false);

}

void MixerComponent::resized() {
	__bounds = getLocalBounds().reduced(5);
	
	Rectangle<int> mixerBounds(__bounds);
	mixerBounds.removeFromTop(mixerBounds.getHeight() * 0.15);
	Rectangle<int> panBounds(mixerBounds.removeFromTop(mixerBounds.getHeight() * 0.2));
	mixerBounds.removeFromTop(mixerBounds.getHeight() * 0.15);
	
	int size = panBounds.getWidth() * 0.045;
	int gap = panBounds.getWidth() * 0.17;

	panBounds.removeFromLeft(panBounds.getWidth() * 0.055);
	__masterPan->setBounds(panBounds.removeFromLeft(size));
	panBounds.removeFromLeft(gap+2);
	__pan1->setBounds(panBounds.removeFromLeft(size));
	panBounds.removeFromLeft(gap-6);
	__pan2->setBounds(panBounds.removeFromLeft(size));
	panBounds.removeFromLeft(gap-7);
	__pan3->setBounds(panBounds.removeFromLeft(size));
	panBounds.removeFromLeft(gap-8);
	__pan4->setBounds(panBounds.removeFromLeft(size));
	
	Rectangle<int> ml(__masterPan->getBounds());
	__panL.setBounds(ml.getX() - __panL.getFont().getHeight() * 1, ml.getY() - 10, __panL.getFont().getHeight() * 2, ml.getHeight());
	__panR.setBounds(ml.getX() + __panR.getFont().getHeight() * 2.8, ml.getY() - 10, __panR.getFont().getHeight() * 2, ml.getHeight());

	Rectangle<int> p1(__pan1->getBounds());
	__pan1L.setBounds(p1.getX() - __pan1L.getFont().getHeight() * 1, p1.getY() - 10, __pan1L.getFont().getHeight() * 2, p1.getHeight());
	__pan1R.setBounds(p1.getX() + __pan1R.getFont().getHeight() * 2.8, p1.getY() - 10, __pan1R.getFont().getHeight() * 2, p1.getHeight());

	Rectangle<int> p2(__pan2->getBounds());
	__pan2L.setBounds(p2.getX() - __pan2L.getFont().getHeight() * 1, p2.getY() - 10, __pan2L.getFont().getHeight() * 2, p2.getHeight());
	__pan2R.setBounds(p2.getX() + __pan2R.getFont().getHeight() * 2.8, p2.getY() - 10, __pan2R.getFont().getHeight() * 2, p2.getHeight());

	Rectangle<int> p3(__pan3->getBounds());
	__pan3L.setBounds(p3.getX() - __pan3L.getFont().getHeight() * 1, p3.getY() - 10, __pan3L.getFont().getHeight() * 2, p3.getHeight());
	__pan3R.setBounds(p3.getX() + __pan3R.getFont().getHeight() * 2.8, p3.getY() - 10, __pan3R.getFont().getHeight() * 2, p3.getHeight());

	Rectangle<int> p4(__pan4->getBounds());
	__pan4L.setBounds(p4.getX() - __pan4L.getFont().getHeight() * 1, p4.getY() - 10, __pan4L.getFont().getHeight() * 2, p4.getHeight());
	__pan4R.setBounds(p4.getX() + __pan4R.getFont().getHeight() * 2.8, p4.getY() - 10, __pan4R.getFont().getHeight() * 2, p4.getHeight());

	size = mixerBounds.getWidth() * 0.15;
	gap = size * 0.4;
	
	__masterMixer->setBounds(mixerBounds.removeFromLeft(size + 10));

	mixerBounds.removeFromLeft(gap);
	__osc1->setBounds(mixerBounds.removeFromLeft(size));

	mixerBounds.removeFromLeft(gap);
	__osc2->setBounds(mixerBounds.removeFromLeft(size));

	mixerBounds.removeFromLeft(gap);
	__osc3->setBounds(mixerBounds.removeFromLeft(size));

	mixerBounds.removeFromLeft(gap);
	__osc4->setBounds(mixerBounds.removeFromLeft(size));
}