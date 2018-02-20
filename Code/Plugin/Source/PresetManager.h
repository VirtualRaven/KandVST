#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
class PresetManager
{
private:
	AudioProcessor* __owner;
	std::map<std::string, XmlElement*> __presets;
	
	String getPresetPath();
public:
	PresetManager(AudioProcessor* owner);
	~PresetManager();

	void RefreshPresets();
	void LoadPreset(std::string name);
	void SavePreset(std::string name);
	bool PresetExists(std::string name);
	std::vector<std::string> GetPresetNames();
	JUCE_LEAK_DETECTOR(PresetManager);
};

#endif // !PRESET_MANAGER_H