#ifndef PLUGIN_GUI_H
#define PLUGIN_GUI_H

#include "PluginProcessor.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Components\MasterPageComponent.h"
#include "Components\OscillatorPageComponent.h"

class PluginGUI  : public AudioProcessorEditor                    
{
public:
	PluginGUI(JuceDemoPluginAudioProcessor& owner);
	~PluginGUI();

	void paint(Graphics& g);
	void resized();

private:
	
	TabbedComponent __tabComponent;
	MidiKeyboardComponent __keyboard;
    //==============================================================================
    JuceDemoPluginAudioProcessor& getProcessor() const
    {
        return static_cast<JuceDemoPluginAudioProcessor&> (processor);
    }

  
};
#endif // !PLUGIN_GUI_H