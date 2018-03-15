#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorComponent.h"

OscillatorComponent::~OscillatorComponent()
{
	delete __oscWaveform;
	//__toggleOsc.removeListener(this);
}

OscillatorComponent::OscillatorComponent(int ID):
IVSTParameters(ID),
__waveformInvalid(false)
{
	__ID = ID;
	//=====================================
	// add and make visible wave sliders
	//=====================================
	addAndMakeVisible(__sineSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SINE")));
	__sineSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__sineSlider->setTextBoxStyle(__sineSlider->NoTextBox, true, 10, 10);
	__sineLabel.setText("SINE", NotificationType::dontSendNotification);
	__sineLabel.attachToComponent(__sineSlider,false);
	__sineLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(__squareSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SQUARE")));
	__squareSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__squareSlider->setTextBoxStyle(__squareSlider->NoTextBox, true, 10, 10);
	__squareLabel.setText("SQUARE", NotificationType::dontSendNotification);
	__squareLabel.attachToComponent(__squareSlider, false);
	__squareLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(__sawSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_SAW")));
	__sawSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__sawSlider->setTextBoxStyle(__sawSlider->NoTextBox, true, 10, 10);
	__sawLabel.setText("SAW", NotificationType::dontSendNotification);
	__sawLabel.attachToComponent(__sawSlider, false);
	__sawLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(__triangleSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_TRI")));
	__triangleSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__triangleSlider->setTextBoxStyle(__triangleSlider->NoTextBox, true, 10, 10);
	__triangleLabel.setText("TRIANGLE", NotificationType::dontSendNotification);
	__triangleLabel.attachToComponent(__triangleSlider, false);
	__triangleLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(__noiseSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_NOISE"))); 
	__noiseSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__noiseSlider->setTextBoxStyle(__noiseSlider->NoTextBox, true, 10, 10);
	__noiseLabel.setText("NOISE", NotificationType::dontSendNotification);
	__noiseLabel.attachToComponent(__noiseSlider, false);
	__noiseLabel.setJustificationType(juce::Justification::centred);
	//=========================================================================
	// add and make visible detune, octaves, etc
	//===============================
	addAndMakeVisible(__octaveSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OCTAVE")));
	__octaveSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__octaveSlider->setTextBoxStyle(__octaveSlider->NoTextBox, true, 0, 0);
	__octaveLabel.setText("OCTAVE", NotificationType::dontSendNotification);
	__octaveLabel.attachToComponent(__octaveSlider, false);
	__octaveLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(__detuneSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "OSC_DETUNE")));
	__detuneSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__detuneSlider->setTextBoxStyle(__detuneSlider->NoTextBox, true, 0, 0);
	__detuneLabel.setText("DETUNE", NotificationType::dontSendNotification);
	__detuneLabel.attachToComponent(__detuneSlider, false);
	__detuneLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(__offsetSlider = new ParameterSlider(*Global->paramHandler->Get<AudioParameterInt>(__ID, "OSC_OFFSET")));
	__offsetSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__offsetSlider->setTextBoxStyle(__offsetSlider->NoTextBox, true, 0, 0);
	__offsetLabel.setText("OFFSET", NotificationType::dontSendNotification);
	__offsetLabel.attachToComponent(__offsetSlider, false);
	__offsetLabel.setJustificationType(juce::Justification::centred);
	//============================================================================

	
	
	Global->paramHandler->addParamaterListener(this, __ID, "OSC_SINE");
	Global->paramHandler->addParamaterListener(this, __ID, "OSC_SAW");
	Global->paramHandler->addParamaterListener(this, __ID, "OSC_SQUARE");
	Global->paramHandler->addParamaterListener(this, __ID, "OSC_TRI");

	//=======
	__oscWaveform = new Image(Image::PixelFormat::RGB, 300, 200, true);

	__waveformComp.setImage(*__oscWaveform);

	WavetableOsc os = WavetableOsc(__ID, 0,0);
	os.renderImage(__oscWaveform);
	__waveformComp.repaint();

	addAndMakeVisible(__waveformComp);
}

void OscillatorComponent::paint(Graphics&){
	
}


void OscillatorComponent::resized(){
	Rectangle<int> bounds(getLocalBounds());
		__waveformComp.setBounds(bounds.removeFromTop(jmin<int>(bounds.getWidth(), bounds.getHeight() / 2)));
	delete __oscWaveform;
	__oscWaveform = nullptr;
	__oscWaveform = new Image(Image::PixelFormat::RGB, __waveformComp.getWidth(), __waveformComp.getHeight(), true);
	WavetableOsc os = WavetableOsc(__ID, 0,0);
	os.renderImage(__oscWaveform);
	__waveformComp.setImage(*__oscWaveform);
	__waveformComp.repaint();

	bounds.removeFromTop(32);
	//Rectangle<int> waveforms(bounds.removeFromTop(jmax<int>(bounds.getWidth() / 5, 100)));
	Rectangle<int> waveforms(bounds.removeFromTop((bounds.getWidth()-100)/5));
	waveforms.removeFromLeft(50);
	int sliderw = (waveforms.getWidth()-50) / 5;

	// waves
	__sineSlider->setBounds(waveforms.removeFromLeft(sliderw));
	__squareSlider->setBounds(waveforms.removeFromLeft(sliderw));
	__sawSlider->setBounds(waveforms.removeFromLeft(sliderw));
	__triangleSlider->setBounds(waveforms.removeFromLeft(sliderw));
	__noiseSlider->setBounds(waveforms.removeFromLeft(sliderw));

	bounds.removeFromTop(32);
	//Rectangle<int> atrSliders(bounds.removeFromTop(jmax<int>(bounds.getWidth() / 3,100)));
	Rectangle<int> atrSliders(bounds.removeFromTop((bounds.getWidth() - 100) / 3));
	atrSliders.removeFromLeft(50);
	sliderw = (atrSliders.getWidth()-50) / 3;

	// detune, offset, octave
	__octaveSlider->setBounds(atrSliders.removeFromLeft(sliderw));
	__detuneSlider->setBounds(atrSliders.removeFromLeft(sliderw));
	__offsetSlider->setBounds(atrSliders.removeFromLeft(sliderw));
}


void OscillatorComponent::timerCallback()
{
	if (!__waveformInvalid)
		stopTimer();
	WavetableOsc os = WavetableOsc(__ID, 0,0);
	os.renderImage(__oscWaveform);
	__waveformComp.repaint();
	__waveformInvalid = false;
}

void OscillatorComponent::parametersChanged(std::vector<std::string>)
{
	__waveformInvalid = true;
	if (!isTimerRunning())
		startTimerHz(60);
}
