#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorComponent.h"

OscillatorComponent::~OscillatorComponent()
{
	delete __oscWaveform;
	//__toggleOsc.removeListener(this);
}

OscillatorComponent::OscillatorComponent(int ID):
IVSTParameters(ID)
{
	__ID = ID;
	//=====================================
	// add and make visible wave sliders
	//=====================================
	addAndMakeVisible(__sineSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SINE")));
	__sineSlider->setSliderStyle(Slider::Rotary);
	__sineSlider->setTextBoxStyle(__sineSlider->NoTextBox, true, 10, 10);
	__sineLabel.setText("SINE", NotificationType::dontSendNotification);
	__sineLabel.attachToComponent(__sineSlider,false);

	addAndMakeVisible(__squareSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SQUARE")));
	__squareSlider->setSliderStyle(Slider::Rotary);
	__squareSlider->setTextBoxStyle(__squareSlider->NoTextBox, true, 10, 10);
	__squareLabel.setText("SQUARE", NotificationType::dontSendNotification);
	__squareLabel.attachToComponent(__squareSlider, false);

	addAndMakeVisible(__sawSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SAW")));
	__sawSlider->setSliderStyle(Slider::Rotary);
	__sawSlider->setTextBoxStyle(__sawSlider->NoTextBox, true, 10, 10);
	__sawLabel.setText("SAW", NotificationType::dontSendNotification);
	__sawLabel.attachToComponent(__sawSlider, false);

	addAndMakeVisible(__triangleSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_TRI")));
	__triangleSlider->setSliderStyle(Slider::Rotary);
	__triangleSlider->setTextBoxStyle(__triangleSlider->NoTextBox, true, 10, 10);
	__triangleLabel.setText("TRIANGLE", NotificationType::dontSendNotification);
	__triangleLabel.attachToComponent(__triangleSlider, false);

	//=========================================================================
	// add and make visible detune, octaves, etc
	//===============================
	addAndMakeVisible(__octaveSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE")));
	__octaveSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	__octaveSlider->setTextBoxStyle(__octaveSlider->NoTextBox, true, 0, 0);
	__octaveLabel.setText("OCTAVE", NotificationType::dontSendNotification);
	__octaveLabel.attachToComponent(__octaveSlider, false);

	addAndMakeVisible(__detuneSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE")));
	__detuneSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	__detuneSlider->setTextBoxStyle(__detuneSlider->NoTextBox, true, 0, 0);
	__detuneLabel.setText("DETUNE", NotificationType::dontSendNotification);
	__detuneLabel.attachToComponent(__detuneSlider, false);

	addAndMakeVisible(__offsetSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET")));
	__offsetSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	__offsetSlider->setTextBoxStyle(__offsetSlider->NoTextBox, true, 0, 0);
	__offsetLabel.setText("OFFSET", NotificationType::dontSendNotification);
	__offsetLabel.attachToComponent(__offsetSlider, false);
	//============================================================================

	addAndMakeVisible(__toggleOsc = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "OSC_MIX_EN")));
	__toggleOsc->setButtonText("Oscillator");
	
	
	//=======
	__oscWaveform = new Image(Image::PixelFormat::RGB, 300, 200, true);
	startTimer(50);
	timerCallback();
}

void OscillatorComponent::paint(Graphics& g){
	if (__oscWaveform != nullptr)
		g.drawImageAt(*__oscWaveform, 10, 50, false);

	/*Path oscPath;
	oscPath.addRectangle(Rectangle<int>(10, 50, 300, 200));
	g.setColour(Colour::fromRGB(20, 20, 20));
	g.fillPath(oscPath);
	g.setColour(Colours::silver);
	g.strokePath(oscPath, PathStrokeType(0.5f));

	g.setColour(Colours::white);
	g.drawText("OSC", oscPath.getBounds(), Justification::centred, false);*/
}


void OscillatorComponent::resized(){
	// toggle button
	__toggleOsc->setBounds(Rectangle<int>(10, 0, 200, 75));
	// waves
	__sineSlider->setBounds(Rectangle<int>(0, 275, 100, 75));
	__squareSlider->setBounds(Rectangle<int>(75, 275, 100, 75));
	__sawSlider->setBounds(Rectangle<int>(150, 275, 100, 75));
	__triangleSlider->setBounds(Rectangle<int>(225, 275, 100, 75));

	// detune, offset, octave
	__octaveSlider->setBounds(Rectangle<int>(0,375,100,75));
	__detuneSlider->setBounds(Rectangle<int>(75,375,100,75));
	__offsetSlider->setBounds(Rectangle<int>(150,375,100,75));
}





void OscillatorComponent::timerCallback()
{
	if (s != __sineSlider->getValue() || sq != __squareSlider->getValue() || sa != __sawSlider->getValue() || tr != __triangleSlider->getValue()) {

		s =  (float) __sineSlider->getValue();
		sq = (float) __squareSlider->getValue();
		sa = (float) __sawSlider->getValue();
		tr = (float) __triangleSlider->getValue();

		WavetableOsc os = WavetableOsc(__ID, 0);
		os.renderImage(__oscWaveform, 300, 200);
		repaint();
	}
}
