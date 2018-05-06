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

#include "PresetManager.h"
#include <algorithm>

String PresetManager::getPresetPath()
{
	File appData = File( File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName()+ String("/KandVST/Presets"));
	appData.createDirectory();
	return appData.getFullPathName();
}

PresetManager::PresetManager(AudioProcessor* owner,GLOBAL* global):
__owner(owner),
__resetPreset(nullptr)
{
	Global = global;

	__precompiledPresets["Default"] = nullptr;
	//Add here to add preCompiled presets
	AddPrecompiledPreset(Resources::Presets::church_xml,sizeof(Resources::Presets::church_xml));
	AddPrecompiledPreset(Resources::Presets::bird_simulator_2019_xml, sizeof(Resources::Presets::bird_simulator_2019_xml));
	AddPrecompiledPreset(Resources::Presets::chip_pluck_xml, sizeof(Resources::Presets::chip_pluck_xml));
	AddPrecompiledPreset(Resources::Presets::chip_pluck_lfo_xml, sizeof(Resources::Presets::chip_pluck_lfo_xml));
	AddPrecompiledPreset(Resources::Presets::chip_pluck_lfo_fast_xml, sizeof(Resources::Presets::chip_pluck_lfo_fast_xml));
	AddPrecompiledPreset(Resources::Presets::chip_stereo_hihat_xml, sizeof(Resources::Presets::chip_stereo_hihat_xml));
}
PresetManager::~PresetManager()
{
	for (auto a : __presets) {
		delete (std::get<1>(a));
	}
}

void PresetManager::RefreshPresets()
{
	File folder(getPresetPath());
	Array<File> presets;
	folder.findChildFiles(presets, File::findFiles, true, "*.xml");

	for (auto a : __filePresets) {
		delete (a);
	}
	__filePresets.clear();
	__presets.clear();

	for (auto&& f : presets) 
	{
		XmlDocument doc(f);
		XmlElement* el(doc.getDocumentElement());
		if (el == nullptr)
			continue;
		if (el->hasTagName("KandVSTPreset")&& el->hasAttribute("name"))
		{
			__filePresets.push_back(el);
			__presets.push_back(std::make_tuple(el->getStringAttribute("name").toStdString(), el));
		}
		
	}

	if (__precompiledPresets["Default"] == nullptr) {
		__resetPreset = new XmlElement("KandVSTPreset");
		// Create Reset preset
		for (auto&& param : __owner->getParameters())
		{
			if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)) {
				XmlElement* paramEl = new XmlElement(String("_") + p->paramID);
				paramEl->setAttribute("value", p->getDefaultValue());
				__resetPreset->addChildElement(paramEl);
			}
		}

		__resetPreset->setAttribute("name", "Default");
		__precompiledPresets["Default"] = __resetPreset;
	}

	for (auto kvp : __precompiledPresets)
	{
		__presets.push_back(std::make_tuple(kvp.first, kvp.second));
	}

	// Sort presets alphabetically, not case sensitive
	std::sort(__presets.begin(), __presets.end(), [](const std::tuple<std::string, XmlElement*>& first, const std::tuple<std::string, XmlElement*>& second)
	{
		// return first < second
		std::string firstName = std::get<0>(first);
		for (auto c : firstName)
			c = static_cast<char>(toupper(c));

		std::string secondName = std::get<0>(second);
		for (auto c : secondName)
			c = static_cast<char>(toupper(c));

		// Default should always be on top
		if (firstName == "Default")
		{
			return true;
		}
		else if (secondName == "Default")
		{
			return false;
		}

		return firstName.compare(secondName) < 0;
	});
}

void PresetManager::LoadPreset(std::string name)
{
	for (size_t i = 0; i < __presets.size(); i++)
	{
		if (std::get<0>(__presets.at(i)) == name)
		{
			LoadPreset(i);
			return;
		}
	}
}

void PresetManager::LoadPreset(int index)
{
	XmlElement *xmlState;
	xmlState = std::get<1>(__presets.at(index)); 
	LoadPreset(xmlState);
	

	__currentPreset = index;
}

void PresetManager::LoadPreset(XmlElement * xmlState)
{
	if (xmlState != nullptr)
	{
		if (xmlState->hasTagName("KandVSTPreset"))
		{
			Global->paramHandler->ClearLinks();
			for (auto* param : __owner->getParameters())
				if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)) {
					XmlElement* child = xmlState->getChildByName(String("_") + p->paramID);
					if (child != nullptr && child->hasAttribute("value"))
						p->setValue((float)child->getDoubleAttribute("value", p->getValue()));
					if (child != nullptr && child->hasAttribute("sender"))
					{
						auto sender = Global->paramHandler->GetParameter(child->getStringAttribute("sender").toStdString());
						auto rec = Global->paramHandler->GetParameter(p->paramID.toStdString());
						if (sender != nullptr && rec != nullptr)
						{
							Global->paramHandler->LinkParameters(sender, rec);
						}

					}
				}
		}
	}

	sendChangeMessage();
}

void PresetManager::SavePreset(XmlElement * xmlState)
{
	auto links = Global->paramHandler->GetLinks();
	for (auto&& param : __owner->getParameters())
	{
		if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)) {
			XmlElement* paramEl = new XmlElement(String("_") + p->paramID);
			paramEl->setAttribute("value", p->getValue());
			auto link = Global->paramHandler->GetSender(param);
			if (link != nullptr)
			{

				if (auto* linkSender = dynamic_cast<AudioProcessorParameterWithID*> (link)) {
					paramEl->setAttribute("sender", linkSender->paramID);
				}
			}
			xmlState->addChildElement(paramEl);
		}
	}
}

void PresetManager::DeletePreset(std::string name) {
	if (isPrecompiled(name))
		return;
	File preset = File(getPresetPath() + String("/") + name + String(".xml"));

	if (preset.exists() && preset.deleteFile()) {
		int index = GetPresetIndex(name);
		if (PresetExists(name)) {
			delete std::get<1>(__presets[index]);
			__presets.erase(__presets.begin() + index);
		}
	}
}

void PresetManager::SavePreset(std::string name)
{	
	if (isPrecompiled(name))
		return;

	XmlElement* el = new XmlElement("KandVSTPreset");
	SavePreset(el);
	el->setAttribute("name", name);
	el->writeToFile(File(getPresetPath() + String("/") + name + String(".xml")), "");
	
	if (PresetExists(name))
	{
		for (auto& preset : __presets)
		{
			if (std::get<0>(preset) == name)
			{
				auto temp = std::get<1>(preset);
				preset = std::make_tuple(name, el);
				delete temp;
				continue;
			}
		}
	}
	else
	{
		__presets.push_back(std::make_tuple(name, el));
	}
}

int PresetManager::GetPresetCount()
{
	return __presets.size();
}

int PresetManager::GetPresetIndex(std::string name)
{
	for (size_t i = 0; i < __presets.size(); i++)
	{
		if (std::get<0>(__presets.at(i)) == name)
		{
			return i;
		}
	}

	return 0;
}

int PresetManager::GetCurrentPreset()
{
	return __currentPreset;
}

bool PresetManager::PresetExists(std::string name)
{
	for (auto preset : __presets)
	{
		if (std::get<0>(preset) == name)
		{
			return true;
		}
	}
	return false;
}

bool PresetManager::isPrecompiled(std::string name)
{
	return __precompiledPresets.count(name)==1;
}

void PresetManager::AddPrecompiledPreset(unsigned char * data, size_t size)
{
	XmlElement*el = XmlDocument::parse(String::createStringFromData(data,size));
	if (el != nullptr && el->hasTagName("KandVSTPreset") && el->hasAttribute("name"))
	{
		__precompiledPresets[el->getStringAttribute("name").toStdString()] = el;
	}
}



std::vector<std::string> PresetManager::GetPresetNames()
{
	std::vector<std::string> presets;

	for (auto preset : __presets)
	{
		presets.push_back(std::get<0>(preset));
	}

	return presets;
}
