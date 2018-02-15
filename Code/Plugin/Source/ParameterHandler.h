#ifndef PARAMETER_HANDLER_H
#define PARAMETER_HANDLER_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include "Log.h"
class ParameterHandler
{
private:
	//JuceDemoPluginAudioProcessor * __owner;
	std::map<String, AudioParameterFloat*> __floatParams;
	std::map<String, AudioParameterInt*> __intParams;
	std::map<String, AudioParameterBool*> __boolParams;
	std::map<String, AudioParameterChoice*> __choiceParams;
	AudioProcessor* __owner;
public:
	ParameterHandler(AudioProcessor& owner);
	~ParameterHandler();
	AudioParameterFloat* RegisterFloat(int iid, String id, String label, float minValue, float maxValue, float defaultvalue);
	AudioParameterInt* RegisterInt(int iid, String id, String label, int minValue, int maxValue, int defaultvalue);
	AudioParameterBool* RegisterBool(int iid, String id, String label, bool defaultvalue);
	AudioParameterChoice* RegisterChoice(int iid, String id, String label, StringArray &choices, int defaultItemIndex);


	template<typename T>
	T* Get(int iid, String id);
	template<typename T>
	std::map<String,T*>* GetAll();
	AudioParameterFloat* GetFloat(String id);
	std::map<String, AudioProcessorParameterWithID*>* GetParameters();
	JUCE_LEAK_DETECTOR(ParameterHandler);
};


#endif //PARAMETER_HANDLER_H

