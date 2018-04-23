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

#include "AboutPageComponent.h"
#include "ParameterButton.h"


AboutPageComponent::~AboutPageComponent() {
	this->setLookAndFeel(nullptr);
}

AboutPageComponent::AboutPageComponent(GLOBAL * global) :
	mc(global),
	ourLookAndFeel(global),
	__themePicker()
{
	Global = global;
	__themeChoice = Global->paramHandler->Get<AudioParameterChoice>(-1, "THEME");

	__info.setReadOnly(true);
	setLookAndFeel(&ourLookAndFeel);
	addAndMakeVisible(mc);


	std::string about = String::createStringFromData(Resources::about_txt, sizeof(Resources::about_txt)).toStdString();
	int c = 0;
	while (true)
	{
		int line = about.find('\n', c);
		if (line != -1)
		{
			mc.addLine(about.substr(c, line - c));
			c = line + 1;
		}
		else 
		{
			mc.addLine(about.substr(c));
			break;
		}
	}

	addAndMakeVisible(__themes);
	__themes.addListener(this);
	StringArray themes = { "Default", "Pink is life", "Go Green", "Orange Juice", "Red" };
	__themes.addItemList(themes, 1);
	__themes.setSelectedItemIndex(__themeChoice->getIndex(), true);
}

void AboutPageComponent::comboBoxChanged(ComboBox * cbox) {
	*__themeChoice = cbox->getSelectedItemIndex();
	getParentComponent()->getParentComponent()->sendLookAndFeelChange();
}

void AboutPageComponent::lookAndFeelChanged() {
	repaint();
}

void AboutPageComponent::paint(Graphics& g) {
	Rectangle<int> aBounds(getLocalBounds().removeFromLeft(800));

	Font header(Font::getDefaultSansSerifFontName(), 30, Font::bold);
	Font text(Font::getDefaultSansSerifFontName(), 16, Font::plain);

	g.setColour(Swatch::background);
	g.fillAll();
	g.setColour(Swatch::background.darker());
	g.drawRect(getLocalBounds(), 2.f);

	g.setColour(Swatch::background.brighter(0.3f));
	g.drawVerticalLine(800, 10, getLocalBounds().getHeight() - 20);

	g.setColour(Swatch::white);

	g.setFont(header);

	g.drawText("THEMES", 800, 10, 480, 30, Justification::centred, false);

	g.setColour(__themePicker.getColour(__themeChoice->getCurrentChoiceName()));
	g.fillRect(990, 100, 100, 50);
}

void AboutPageComponent::resized() {
	Rectangle<int> themeBounds(getLocalBounds().removeFromRight(450));
	themeBounds.removeFromTop(50);
	__themes.setBounds(themeBounds.removeFromTop(50).reduced(8));
	mc.setBounds(getLocalBounds().removeFromLeft(getLocalBounds().getWidth() - 450));
}

void AboutPageComponent::RegisterParameters(int ID, GLOBAL *global) {
	StringArray themes = { "Default", "Pink", "Green", "Orange", "Red" };
	global->paramHandler->RegisterChoice(ID, "THEME", "THEME", themes, 0);
}

