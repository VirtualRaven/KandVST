#ifndef PARAMETER_HANDLER_H
#define PARAMETER_HANDLER_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include "Global.h"
#include "Log.h"
class ParameterHandler
{
private:
	//JuceDemoPluginAudioProcessor * __owner;
	std::map<String, AudioParameterFloat*> __floatParams;
	AudioProcessor* __owner;
public:
	ParameterHandler(AudioProcessor& owner);
	~ParameterHandler();
	AudioParameterFloat* RegisterFloat(int iid, String id, String label, float minValue, float maxValue, float defaultvalue);
	AudioParameterFloat* GetFloat(int iid, String id);
	AudioParameterFloat* GetFloat(String id);
	std::map<String, AudioParameterFloat*>* GetFloats();

};

#endif //PARAMETER_HANDLER_H
