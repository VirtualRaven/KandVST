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

#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "Resources_files.h"
class PresetManager : public ChangeBroadcaster
{
private:
	AudioProcessor* __owner;
	std::vector<std::tuple<std::string, XmlElement*>> __presets;
	std::vector<XmlElement*> __filePresets;
	std::map<std::string, XmlElement*> __precompiledPresets;
	String getPresetPath();
	int __currentPreset = 0;
	XmlElement* __resetPreset;
public:
	PresetManager(AudioProcessor* owner,GLOBAL* global);
	~PresetManager();
	
	void RefreshPresets();
	void LoadPreset(std::string name);
	void LoadPreset(int index);
	void SavePreset(std::string name);
	void DeletePreset(std::string name);
	int GetPresetCount();
	int GetPresetIndex(std::string name);
	int GetCurrentPreset();
	bool PresetExists(std::string name);
	bool isPrecompiled(std::string name);
	void AddPrecompiledPreset(unsigned char* data, size_t size);
	void LoadPreset(XmlElement * xmlState);
	void SavePreset(XmlElement * xmlState);
	void DeletePreset(XmlElement * xmlState);

	GLOBAL * Global;
	std::vector<std::string> GetPresetNames();
	JUCE_LEAK_DETECTOR(PresetManager);
};

#endif // !PRESET_MANAGER_H