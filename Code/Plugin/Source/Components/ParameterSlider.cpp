/*
==============================================================================

This file is part of the JUCE library.
Copyright (c) 2017 - ROLI Ltd.

JUCE is an open source library subject to commercial or open-source
licensing.

By using JUCE, you agree to the terms of both the JUCE 5 End-User License
Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
27th April 2017).

End User License Agreement: www.juce.com/juce-5-licence
Privacy Policy: www.juce.com/juce-5-privacy-policy

Or: You may also use this code under the terms of the GPL v3 (see
www.gnu.org/licenses).

JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
DISCLAIMED.

==============================================================================
*/


#include "ParameterSlider.h"



void ParameterSlider::mouseUp(const MouseEvent & event)
{
	Slider::mouseUp(event);
	if (event.mouseWasClicked() && event.mods.testFlags(ModifierKeys::rightButtonModifier))
	{
		if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (&param)) {
			PopupMenu pm = PopupMenu();
			if (!isLinked()) {
				pm.addSectionHeader("Links");
				for (auto p : Global->paramHandler->FindSimilar(pid))
				{
					String idString = String(p.first + 1);
					if (p.first == -1)
						idString = "Master";
					pm.addItem(p.first + 100, idString + String(" - ")+  p.second->name);
				}
			
				pm.showMenuAsync(PopupMenu::Options(), ModalCallbackFunction::create([this](int ind) {
					if (ind > 0) {
						Linkable::Link(ind - 100);
						setEnabled(false);
					}
				}));
			}
			else 
			{
				pm.addItem(1,"Unlink");
				pm.showMenuAsync(PopupMenu::Options(), ModalCallbackFunction::create([this](int ind) {
					if (ind == 1) {
						Unlink();
						setEnabled(true);

					}
				}));
			}
		}
	}
}

ParameterSlider::ParameterSlider(AudioProcessorParameter& p,GLOBAL*global):
	 Slider(p.getName(256)), param(p),
	Linkable(&p,global),
	__progressStart(ParameterSlider::ProgressStart::Start),
	__wave(0)
{
	Global = global;
	setRange(0.0, 1.0, 0.0);
	startTimerHz(30);
	updateSliderPos();
	LinkCouldHaveChanged();

	// Defaults for AudioParameterChoice
	if (auto ps = dynamic_cast<AudioParameterChoice*>(&(p)))
	{
		// Limits
		float angDelta = (float_Pi / 4.0f) * (-0.2f * ps->choices.size() + 1.9f);
		float halfMaxAngle = (angDelta * (ps->choices.size() - 1)) / 2.0f;
		setRotaryParameters((2.0f*float_Pi) - halfMaxAngle, (2.0f*float_Pi) + halfMaxAngle, true);

		// Style
		setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
		setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
	}
}


ParameterSlider::~ParameterSlider()
{
}

void ParameterSlider::valueChanged()
{ 
	param.setValueNotifyingHost((float)Slider::getValue()); 
}

void ParameterSlider::timerCallback()
{ 
	updateSliderPos(); 
	this->setTooltip(getTextFromValue(param.getValue()));
}

void ParameterSlider::startedDragging()
{ 
	if (getSliderStyle() != Slider::LinearHorizontal && getSliderStyle() != Slider::LinearVertical) {
		__mousePos = Desktop::getInstance().getMainMouseSource().getScreenPosition();
		setMouseCursor(MouseCursor::NoCursor);
	}
	param.beginChangeGesture(); 
}
void ParameterSlider::stoppedDragging()
{
	if (getSliderStyle() != Slider::LinearHorizontal && getSliderStyle() != Slider::LinearVertical) {
		Desktop::getInstance().getMainMouseSource().setScreenPosition(__mousePos);
		setMouseCursor(MouseCursor::NormalCursor);
	}
	param.endChangeGesture(); 
}

void ParameterSlider::setDrawProgress(ProgressStart startLocation)
{
	__progressStart = startLocation;
}

ParameterSlider::ProgressStart ParameterSlider::getDrawProgress()
{
	return __progressStart;
}

double ParameterSlider::getValueFromText(const String& text)
{ 
	return param.getValueForText(text); 
}
String ParameterSlider::getTextFromValue(double value)
{ 
	return param.getText((float)value, 1024); 
}

void ParameterSlider::updateSliderPos()
{
	const float newValue = param.getValue();

	if (newValue != (float)Slider::getValue() && !isMouseButtonDown())
		Slider::setValue(newValue, NotificationType::dontSendNotification);
}

void ParameterSlider::mouseDoubleClick(const MouseEvent & event)
{
	param.setValueNotifyingHost(param.getDefaultValue());

}

void ParameterSlider::Link(int id)
{
	Linkable::Link(id);
}


void ParameterSlider::RemoveLink() {
	Unlink();
}

void ParameterSlider::LinkCouldHaveChanged()
{
		this->setEnabled(!isLinked());
}

void ParameterSlider::setWaveType(int wave) {
	__wave = wave;
}

int ParameterSlider::getWaveType() {
	return __wave;
}


