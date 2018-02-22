#include "PresetManager.h"



String PresetManager::getPresetPath()
{
	File appData = File( File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName()+ String("/KandVST/Presets"));
	appData.createDirectory();
	return appData.getFullPathName();
}

PresetManager::PresetManager(AudioProcessor* owner):
__owner(owner)
{
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
	__presets.clear();

	for (auto&& f : presets) 
	{
		XmlDocument doc(f);
		XmlElement* el(doc.getDocumentElement());
		if (el == nullptr)
			continue;
		if (el->hasTagName("KandVSTPreset")&& el->hasAttribute("name"))
		{
			__presets.push_back(std::make_tuple(el->getStringAttribute("name").toStdString(), el));
		}
		
	}

}

void PresetManager::LoadPreset(std::string name)
{
	for (int i = 0; i < __presets.size(); i++)
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

	if (xmlState != nullptr)
	{
		if (xmlState->hasTagName("KandVSTPreset"))
		{
			for (auto* param : __owner->getParameters())
				if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)) {
					XmlElement* child = xmlState->getChildByName(String("_") + p->paramID);
					if (child != nullptr && child->hasAttribute("value"))
						p->setValue((float)child->getDoubleAttribute("value", p->getValue()));
				}
		}
	}

	__currentPreset = index;
}

void PresetManager::SavePreset(std::string name)
{	
	for (int i = 0; i < __presets.size(); i++)
	{
		if (std::get<0>(__presets.at(i)) == name)
		{
			delete std::get<1>(__presets.at(i));
			__presets.erase(__presets.begin() + i);
		}
	}

	XmlElement* el = new XmlElement("KandVSTPreset");
	el->setAttribute("name", name);
	for (auto&& param : __owner->getParameters())
	{
		if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param)) {
			XmlElement* paramEl = new XmlElement(String("_") + p->paramID);
			paramEl->setAttribute("value",p->getValue());
			el->addChildElement(paramEl);
		}
	}

	el->writeToFile(File(getPresetPath() + String("/") + name + String(".xml")), "");
	
	if (PresetExists(name))
	{
		for (auto preset : __presets)
		{
			if (std::get<0>(preset) == name)
			{
				std::get<1>(preset) = el;
				continue;
			}
		}
	}
	else
	{
		std::make_tuple(name, el);
	}
}

int PresetManager::GetPresetCount()
{
	return __presets.size();
}

int PresetManager::GetPresetIndex(std::string name)
{
	for (int i = 0; i < __presets.size(); i++)
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

std::vector<std::string> PresetManager::GetPresetNames()
{
	std::vector<std::string> presets;

	for (auto preset : __presets)
	{
		presets.push_back(std::get<0>(preset));
	}

	return presets;
}
