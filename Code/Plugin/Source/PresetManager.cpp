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


void PresetManager::SavePreset(std::string name)
{	
	for (size_t i = 0; i < __presets.size(); i++)
	{
		if (std::get<0>(__presets.at(i)) == name)
		{
			delete std::get<1>(__presets.at(i));
			__presets.erase(__presets.begin() + i);
		}
	}

	XmlElement* el = new XmlElement("KandVSTPreset");
	SavePreset(el);
	el->setAttribute("name", name);
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



std::vector<std::string> PresetManager::GetPresetNames()
{
	std::vector<std::string> presets;

	for (auto preset : __presets)
	{
		presets.push_back(std::get<0>(preset));
	}

	return presets;
}
