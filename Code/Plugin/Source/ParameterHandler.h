#ifndef PARAMETER_HANDLER_H
#define PARAMETER_HANDLER_H

#define INDEX_CHANGE_SIZE 100

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterListener.h"
#include <map>
#include "Log.h"
class ParameterHandler : public AudioProcessorListener, private AsyncUpdater
{
private:
	//JuceDemoPluginAudioProcessor * __owner;
	std::map<String, AudioParameterFloat*> __floatParams;
	std::map<String, AudioParameterInt*> __intParams;
	std::map<String, AudioParameterBool*> __boolParams;
	std::map<String, AudioParameterChoice*> __choiceParams;
	AudioProcessor* __owner;
	std::map<std::string, std::vector<ParameterListener*>> __parameterListeners;
	int indexChange[INDEX_CHANGE_SIZE];
	int head = 0, tail = 0;
	// Inherited via AudioProcessorListener
	virtual void audioProcessorParameterChanged(AudioProcessor * processor, int parameterIndex, float newValue) override;
	virtual void audioProcessorChanged(AudioProcessor * processor) override;

	// Inherited via AsyncUpdater
	virtual void handleAsyncUpdate() override;

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
	void addParamaterListener(ParameterListener*, int intId,std::string id);
	void addParamaterListener(ParameterListener*, int intId,std::vector<std::string> id);
	JUCE_LEAK_DETECTOR(ParameterHandler);

	
};


#endif //PARAMETER_HANDLER_H

