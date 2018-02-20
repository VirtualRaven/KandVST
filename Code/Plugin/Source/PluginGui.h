#ifndef PLUGIN_GUI_H
#define PLUGIN_GUI_H

#include "PluginProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include "Components\ConsoleComponent.h"
#include "Components\MasterPageComponent.h"
#include "Components\OscillatorPageComponent.h"

class PluginGUI  : public AudioProcessorEditor                    
{
public:
	PluginGUI(PluginProcessor& owner);
	~PluginGUI();

	void paint(Graphics& g);
	void resized();

private:
	
	TabbedComponent __tabComponent;
	MidiKeyboardComponent __keyboard;
	ConsoleComponent __cc;
    //==============================================================================
	PluginProcessor& getProcessor() const
    {
        return static_cast<PluginProcessor&> (processor);
    }

  
};
#endif // !PLUGIN_GUI_H