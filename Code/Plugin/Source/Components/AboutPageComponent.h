#ifndef ABOUT_PAGE_COMPONENT_H
#define ABOUT_PAGE_COMPONENT_H

#include "JuceHeader.h"
#include "Global.h"
#include "ThemePicker.h"
#include "OurLookAndFeel.h"

class AboutPageComponent : public Component, public ComboBox::Listener{
public :
	AboutPageComponent(GLOBAL * global);
	~AboutPageComponent();

	void paint(Graphics& g) override;
	void resized() override;
	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	virtual void lookAndFeelChanged() override;

	static void RegisterParameters(int id, GLOBAL *global);
private:
	OurLookAndFeel ourLookAndFeel;
	GLOBAL * Global;
	TextEditor __info;
	ComboBox __themes;
	AudioParameterChoice * __themeChoice;
	String __about;
	ThemePicker __themePicker;
	Colour __themeColour;
};



#endif ABOUT_PAGE_COMPONENT_H