#include "AboutPageComponent.h"


AboutPageComponent::~AboutPageComponent() {
	this->setLookAndFeel(nullptr);
}

AboutPageComponent::AboutPageComponent(GLOBAL * global) :
	ourLookAndFeel(),
	mc(global)
{
	Global = global;
	__info.setReadOnly(true);
	setLookAndFeel(&ourLookAndFeel);
	File about = File(juce::File::getCurrentWorkingDirectory().getParentDirectory().getFullPathName() + String("/about.txt"));
	FileInputStream * str = about.createInputStream();
	if (str->openedOk()) {
		while (str->getNumBytesRemaining() != 0) {
			mc.addLine(str->readNextLine().toStdString());
			//__about.append(str->readNextLine(), 1000);
			//__about.append("\n", 2);
		}
	}
	addAndMakeVisible(mc);
	addAndMakeVisible(__themes);
	__themes.addListener(this);
	StringArray themes = { "Default", "Pink is life", "Go Green" };
	__themes.addItemList(themes, 1);
	__themes.setSelectedItemIndex(0, true);

}

void AboutPageComponent::comboBoxChanged(ComboBox * cbox) {
	ourLookAndFeel.setThemeId(cbox->getSelectedItemIndex());
	getParentComponent()->sendLookAndFeelChange();
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
	g.drawVerticalLine(800, 10, getLocalBounds().getHeight());

	g.setColour(Swatch::white);

	g.setFont(header);
	//g.drawText("ABOUT", 0, 10, 800, 30, Justification::centred, false);
	g.drawText("THEMES", 800, 20, 480, 30, Justification::centred, false);

	//g.setFont(text);
	//g.drawMultiLineText(__about, 8, 70, 800);

	g.setColour(findColour(1337));
	g.fillRect(990, 100, 100, 50);
}

void AboutPageComponent::resized() {
	Rectangle<int> themeBounds(getLocalBounds().removeFromRight(450));
	themeBounds.removeFromTop(50);
	__themes.setBounds(themeBounds.removeFromTop(50).reduced(8));
	mc.setBounds(getLocalBounds().removeFromLeft(getLocalBounds().getWidth() - 450));
}
