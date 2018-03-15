#ifndef PARAMETER_HANDLER_H
#define PARAMETER_HANDLER_H

#define INDEX_CHANGE_SIZE 100

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterListener.h"
#include <map>
#include "Log.h"
class ParameterHandler : private AsyncUpdater,private Timer
{
private:
	std::map<AudioProcessorParameter *, float> __changeList;
	//JuceDemoPluginAudioProcessor * __owner;
	std::map<String, AudioParameterFloat*> __floatParams;
	std::map<String, AudioParameterInt*> __intParams;
	std::map<String, AudioParameterBool*> __boolParams;
	std::map<String, AudioParameterChoice*> __choiceParams;
	AudioProcessor* __owner;
	std::map<std::string, std::vector<ParameterListener*>> __parameterListeners;
	std::map<AudioProcessorParameter*, std::vector<AudioProcessorParameter*>> __parameterLinks;
	int indexChange[INDEX_CHANGE_SIZE];
	int head = 0, tail = 0;
	// Inherited via AudioProcessorListener
	void audioProcessorParameterChanged(AudioProcessor * processor, int parameterIndex, float newValue);


	// Inherited via AsyncUpdater
	virtual void handleAsyncUpdate() override;

	std::string createParameterId(int intId, std::string id);

public:
	ParameterHandler(AudioProcessor& owner);
	~ParameterHandler();
	AudioParameterFloat* RegisterFloat(int iid, String id, String label, float minValue, float maxValue, float defaultvalue);
	AudioParameterInt* RegisterInt(int iid, String id, String label, int minValue, int maxValue, int defaultvalue);
	AudioParameterBool* RegisterBool(int iid, String id, String label, bool defaultvalue);
	AudioParameterChoice* RegisterChoice(int iid, String id, String label, StringArray &choices, int defaultItemIndex);
	AudioProcessorParameter* GetParameter(int iid, std::string id);
	AudioProcessorParameter* GetParameter(std::string id);
	bool LinkParameters(AudioProcessorParameter * sender, AudioProcessorParameter * receiver);
	bool LinkParameters(int sender_intId, std::string sender, int receiver_intId,std::string receiver);
	bool RemoveLink(AudioProcessorParameter * sender, AudioProcessorParameter * receiver);
	bool RemoveLink(int sender_intId, std::string sender, int receiver_intId, std::string receiver);
	std::map<int,AudioProcessorParameterWithID*> FindSimilar(AudioProcessorParameterWithID*);
	const std::map<AudioProcessorParameter*, std::vector<AudioProcessorParameter*>> GetLinks();
	AudioProcessorParameter* GetSender(AudioProcessorParameter* link);


	template<typename T>
	T* Get(int iid, String id);
	template<typename T>
	std::map<String,T*>* GetAll();
	AudioParameterFloat* GetFloat(String id);
	std::map<String, AudioProcessorParameterWithID*>* GetParameters();
	void addParamaterListener(ParameterListener*, int intId,std::string id);
	void addParamaterListener(ParameterListener*, int intId,std::vector<std::string> id);
	JUCE_LEAK_DETECTOR(ParameterHandler);

	

	// Inherited via Timer
	virtual void timerCallback() override;

};


#endif //PARAMETER_HANDLER_H

