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



#include "PluginProcessor.h"
#include "PluginGUI.h"
#include "../Util/Global.h"
//==============================================================================
PluginGUI::PluginGUI(PluginProcessor& owner, GLOBAL* global)
	: AudioProcessorEditor(owner),
	ourLookAndFeel(global),
	__tabComponent(TabbedButtonBar::Orientation::TabsAtTop),
	__keyboard(owner.keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard),
	__owner(&owner),
	__initialized(false),
	__guiInit(false)
#ifdef CONSOLE
	,__cc(global)
#endif
{
	Global=global;
	setResizable(false, false);
	setSize(1280, 720);
	__loadingImage.setImage(ImageFileFormat::loadFrom(Resources::Images::loading_png,sizeof(Resources::Images::loading_png)));

	addAndMakeVisible(__loadingImage);
	__loadingImage.setBounds(getLocalBounds());
	addKeyListener(this);
	startTimerHz(60);
}


void PluginGUI::InitializeGui()
{
	if (__guiInit)
		return;
	__guiInit = true;
	const MessageManagerLock mmLock;
	removeAllChildren();
	setLookAndFeel(&ourLookAndFeel);
	__tabComponent.setColour(TabbedComponent::ColourIds::outlineColourId, Colour::fromRGBA(0, 0, 0, 0));
	__tabComponent.setTabBarDepth(35);
	__tabComponent.setOutline(0);
	__tabComponent.getTabbedButtonBar().setColour(TabbedButtonBar::ColourIds::tabOutlineColourId, Colours::transparentWhite);
	__tabComponent.addTab("M", Colour::fromRGB(200,200,200), new MasterComponent(Global), true);

	for (int i = 0; i < 4; i++)
	{
		__tabComponent.addTab(std::to_string(i + 1), Colour::fromRGB(1,1,1), new OscillatorPageComponent(i, Global), true);
	}
	
	__tabComponent.setCurrentTabIndex(0);
#ifdef CONSOLE
	__tabComponent.addTab("C", Colours::darkgrey, &__cc, true);
#endif
	__tabComponent.addTab("ABOUT", Colours::darkgrey, new AboutPageComponent(Global), true);

	addAndMakeVisible(__tabComponent);
	addAndMakeVisible(__keyboard);
	__keyboard.setKeyWidth(__keyboard.getKeyWidth() + 20.0f);
	__keyboard.setLowestVisibleKey(48);
	__keyboard.setOctaveForMiddleC(4);
	__keyboard.setKeyPressBaseOctave(5);
}

bool PluginGUI::keyPressed(const KeyPress & /*key*/, Component * /*originatingComponent*/)
{
	return false;
}

bool PluginGUI::keyStateChanged(bool isKeyDown, Component * /*originatingComponent*/)
{
	return __keyboard.keyStateChanged(isKeyDown);
}

void PluginGUI::timerCallback()
{
	if (!__guiInit&&__owner->isReady() && !__initialized) {
		InitializeGui();
		__initialized = true;
	}

	// Messagebox queue:
	if (Global->IsMessageBoxInQueue())
	{
		GLOBAL::MessageBoxInfo info = Global->GetMessageBoxInfo();
		NativeMessageBox::showMessageBox(info.icon, info.title, info.message);
	}
}



PluginGUI::~PluginGUI()
{
	//Has to unset our lookAndFeel before it is destroyed
	this->setLookAndFeel(nullptr);
}

//==============================================================================
void PluginGUI::paint (Graphics& g)
{
    g.setColour (Colour::fromRGB(36,36,36));
    g.fillAll();

	Image titleImage = ImageFileFormat::loadFrom(Resources::Images::Title3_png, sizeof(Resources::Images::Title3_png));
	g.setOpacity(1.0f);
	g.drawImageAt(titleImage, getLocalBounds().getWidth() - titleImage.getBounds().getWidth() - 10, 2, false);

}

void PluginGUI::resized()
{
	Rectangle<int> localBounds (getLocalBounds().reduced(8));

	__keyboard.setBounds(localBounds.removeFromBottom(100));
	__keyboard.setCentrePosition(localBounds.getCentreX(), __keyboard.getBounds().getBottom()-(__keyboard.getHeight()/2));	
	localBounds.removeFromBottom(8); //Padding
	__tabComponent.setBounds(localBounds);

}



