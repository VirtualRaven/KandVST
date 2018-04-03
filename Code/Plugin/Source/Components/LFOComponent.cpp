#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOComponent.h"

LFOComponent::~LFOComponent()
{
}

LFOComponent::LFOComponent(int ID,GLOBAL * global):
IVSTParameters(ID)
{
	Global=global;
	__ID = ID;
	int boxWidth = 50;
	addAndMakeVisible(__ratio = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_RATIO"),Global));
	__ratio->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__ratio->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__ratioLabel.setText("RATIO", NotificationType::dontSendNotification);
	__ratioLabel.attachToComponent(__ratio, false);
	__ratioLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__type = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "LFO_TYPE"), Global));
	__type->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__type->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__type->setDrawProgress(ParameterSlider::ProgressStart::SingleDot);
	__typeLabel.setText("TYPE", NotificationType::dontSendNotification);
	__typeLabel.attachToComponent(__type, false);
	__typeLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__amp = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "LFO_AMOUNT"), Global));
	__amp->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__amp->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, boxWidth, 15);
	__ampLabel.setText("GAIN", NotificationType::dontSendNotification);
	__ampLabel.attachToComponent(__amp, false);
	__ampLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(__toggleLfo = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN")));
	__toggleLfo->setButtonText("LFO " + std::to_string(__ID+1));

	addAndMakeVisible(__toggleInvert = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_INV")));
	__toggleInvert->setButtonText("INVERT");

	addAndMakeVisible(__onPress = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_PRESS")));
	__onPress->setButtonText("ON PRESS");

	//setSize(260, 140);
}

void LFOComponent::paint(Graphics& g){
	g.setColour(Swatch::outlineColour);

	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = __bounds.getHeight() * 0.2;
	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));


	if (!*Global->paramHandler->Get<AudioParameterBool>(__ID, "LFO_EN")) { //if disabled
		__ratio->setEnabled(false);
		__type->setEnabled(false);
		__amp->setEnabled(false);
		__toggleInvert->setEnabled(false);
		__onPress->setEnabled(false);

		__ratioLabel.setColour(Label::textColourId, Swatch::disabled);
		__typeLabel.setColour(Label::textColourId, Swatch::disabled);
		__ampLabel.setColour(Label::textColourId, Swatch::disabled);
	}
	else { //if enabled
		__ratio->setEnabled(true);
		__type->setEnabled(true);
		__amp->setEnabled(true);
		__toggleInvert->setEnabled(true);
		__onPress->setEnabled(true);

		__ratioLabel.setColour(Label::textColourId, Colours::white);
		__typeLabel.setColour(Label::textColourId, Colours::white);
		__ampLabel.setColour(Label::textColourId, Colours::white);
	}

}

void LFOComponent::resized(){
	__bounds = getLocalBounds();
	
	int fontHeight = __bounds.getHeight() * 0.2;
	Rectangle<int> lfoButtonBounds(0,0, __bounds.getWidth(), fontHeight);
	__toggleLfo->setBounds(lfoButtonBounds);

	Rectangle<int> sliders(__bounds.reduced(8));
	int size = sliders.getWidth() / 4;
	sliders.removeFromTop(fontHeight);
	sliders.removeFromTop(__ampLabel.getFont().getHeight());
	__ratio->setBounds(sliders.removeFromLeft(size));
	__type->setBounds(sliders.removeFromLeft(size));
	__amp->setBounds(sliders.removeFromLeft(size));
	__toggleInvert->setBounds(sliders.removeFromTop(20));
	sliders.removeFromTop(20);
	__onPress->setBounds(sliders.removeFromTop(20));

}