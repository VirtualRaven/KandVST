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

#include "DelayComponent.h"
#include "DelayComponent.h"

DelayComponent::~DelayComponent() {
}

DelayComponent::DelayComponent(GLOBAL* global) :
	IVSTParameters(-1)
{
	Global = global;
	__delaySpeed = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "EX_DELAYLENGTH"), Global);
	__delaySeconds = new ParameterSlider(*Global->paramHandler->Get<AudioParameterFloat>(__ID, "EX_DELAYMULTI"), Global);
	__toggleDelay = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "DELAY_EN"));

	addAndMakeVisible(__delaySpeed);
	addAndMakeVisible(__delaySeconds);
	addAndMakeVisible(__toggleDelay);

	__delaySpeed->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__delaySpeed->setTextBoxStyle(__delaySpeed->TextBoxBelow, true, 50, 15);
	__speedLabel.setText("RATE", NotificationType::dontSendNotification);
	__speedLabel.attachToComponent(__delaySpeed, false);
	__speedLabel.setJustificationType(Justification:: centred);

	__delaySeconds->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	__delaySeconds->setTextBoxStyle(__delaySpeed->TextBoxBelow, true, 50, 15);
	__secondsLabel.setText("TIME", NotificationType::dontSendNotification);
	__secondsLabel.attachToComponent(__delaySeconds, false);
	__secondsLabel.setJustificationType(Justification::centred);

	__toggleDelay->setButtonText("DELAY");

	addAndMakeVisible(__toggleSync = new ParameterButton(*Global->paramHandler->Get<AudioParameterBool>(__ID, "DELAY_SYNC")));
	__toggleSync->setButtonText("SYNC");

}

void DelayComponent::paint(Graphics & g) {
	g.setColour(Colour::fromRGB(60, 60, 60));
	int width = getLocalBounds().getWidth();
	int height = getLocalBounds().getHeight();
	int fontHeight = getLocalBounds().getHeight() * 0.2;
	int fontSize = getLocalBounds().getHeight() * 0.16;

	g.drawRect(Rectangle<int>(0, 0, width, height), 2.0f);
	g.fillRect(Rectangle<int>(0, 0, width, fontHeight));

	// if enabled
	if (*Global->paramHandler->Get<AudioParameterBool>(__ID, "DELAY_EN")) {
		__delaySpeed->setEnabled(true);
		__delaySeconds->setEnabled(true);
		__toggleSync->setEnabled(true);
		
	}
	else //disabled
	{ 
		__delaySpeed->setEnabled(false);
		__delaySeconds->setEnabled(false);
		__toggleSync->setEnabled(false);
	}

}

void DelayComponent::resized() {
	__bounds = getLocalBounds();
	
	int fontHeight = __bounds.getHeight() * 0.2;
	__toggleDelay->setBounds(__bounds.removeFromTop(fontHeight));


	

	Rectangle<int> __delayBounds(__bounds.reduced(8));
	__delayBounds.removeFromTop(__secondsLabel.getFont().getHeight());

	

	int __size = (__delayBounds.getWidth() * 0.5f);
	int height = __bounds.getHeight()*0.5;
	__delaySpeed->setBounds(__delayBounds.removeFromLeft(__size).removeFromTop(height).removeFromBottom(height));
	__delaySeconds->setBounds(__delayBounds.removeFromTop(height).removeFromBottom(height));

	Rectangle<int> syncBounds = __bounds.reduced(8).removeFromBottom(fontHeight*0.5).removeFromRight(__bounds.reduced(8).getWidth()*0.75);
	__toggleSync->setBounds(syncBounds.removeFromLeft(syncBounds.getWidth() * (2.0/3.0)));
	//__toggleSync->setBounds(__delayBounds.removeFromBottom(8));
}