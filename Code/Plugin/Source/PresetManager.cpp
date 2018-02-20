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
		delete (a.second);
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
					__presets[el->getStringAttribute("name").toStdString()] = el;
		}
		
	}

}

void PresetManager::LoadPreset(std::string name)
{
	XmlElement* xmlState(__presets[name]);
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
}

void PresetManager::SavePreset(std::string name)
{	
	delete __presets[name];
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
	__presets[name] = el;
}

bool PresetManager::PresetExists(std::string name)
{
	return __presets.count(name) == 1;
}

std::vector<std::string> PresetManager::GetPresetNames()
{
	std::vector<std::string> presets;

	for (auto p : __presets)
	{
		presets.push_back(p.first);
	}

	return presets;
}
