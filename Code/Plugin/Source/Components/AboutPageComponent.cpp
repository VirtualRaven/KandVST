#include "AboutPageComponent.h"

AboutPageComponent::AboutPageComponent(GLOBAL * global)
{
	Global = global;
	__info.setReadOnly(true);

	File about = File(juce::File::getCurrentWorkingDirectory().getParentDirectory().getFullPathName() + String("/Resources/about.txt"));
	FileInputStream * str = about.createInputStream();

	if (str->openedOk()) {
		while (str->getNumBytesRemaining() != 0) {
			__about.append(str->readNextLine(), 1000);
			__about.append("\n", 2);
		}
	}
	
}

AboutPageComponent::~AboutPageComponent() {

}

void AboutPageComponent::paint(Graphics& g) {
	
	Rectangle<int> aBounds(getLocalBounds().removeFromLeft(800));
	Rectangle<int> themeBounds(getLocalBounds().removeFromRight(400));
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
	g.drawText("ABOUT", 0, 10, 800, 30, Justification::centred, false);
	g.drawText("THEMES", 800, 20, 480, 30, Justification::centred, false);

	g.setFont(text);
	g.drawMultiLineText(__about, 8, 70, 800);
}

void AboutPageComponent::resized() {

}
