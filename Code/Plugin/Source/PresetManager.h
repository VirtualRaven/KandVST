#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
class PresetManager : public ChangeBroadcaster
{
private:
	AudioProcessor* __owner;
	//std::map<std::string, XmlElement*> __presets;
	std::vector<std::tuple<std::string, XmlElement*>> __presets;
	String getPresetPath();
	int __currentPreset = 0;

public:
	PresetManager(AudioProcessor* owner);
	~PresetManager();

	void RefreshPresets();
	void LoadPreset(std::string name);
	void LoadPreset(int index);
	void SavePreset(std::string name);
	int GetPresetCount();
	int GetPresetIndex(std::string name);
	int GetCurrentPreset();
	bool PresetExists(std::string name);
	std::vector<std::string> GetPresetNames();
	JUCE_LEAK_DETECTOR(PresetManager);
};

#endif // !PRESET_MANAGER_H