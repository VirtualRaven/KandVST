#ifndef PLUGIN_GUI_H
#define PLUGIN_GUI_H

#include "PluginProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include "Components\ConsoleComponent.h"
#include "Components\MasterPageComponent.h"
#include "Components\OscillatorPageComponent.h"
#include "OurLookAndFeel.h"

class PluginGUI  : public AudioProcessorEditor, private KeyListener, private Timer            
{
public:
	PluginGUI(PluginProcessor& owner);
	~PluginGUI();

	void paint(Graphics& g);
	void resized();
	void InitializeGui();

private:
	OurLookAndFeel ourLookAndFeel; //Must be on top
	ImageComponent  __loadingImage;
	TabbedComponent __tabComponent;
	MidiKeyboardComponent __keyboard;
	ConsoleComponent __cc;
	PluginProcessor* __owner;
    //==============================================================================
	PluginProcessor& getProcessor() const
    {
        return static_cast<PluginProcessor&> (processor);
    }

	void updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo pos);

	// Inherited via KeyListener
	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
	virtual bool keyStateChanged(bool isKeyDown, Component* originatingComponent);


	// Inherited via Timer
	virtual void timerCallback() override;
	bool __guiInit;
};
#endif // !PLUGIN_GUI_H