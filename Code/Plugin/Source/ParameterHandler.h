#ifndef PARAMETER_HANDLER_H
#define PARAMETER_HANDLER_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
class ParameterHandler
{
private:
	//JuceDemoPluginAudioProcessor * __owner;
	std::map<String, AudioParameterFloat*> __floatParams;
	AudioProcessor* __owner;
public:
	ParameterHandler(AudioProcessor& owner);
	~ParameterHandler();
	AudioParameterFloat* RegisterFloat(String id, String label, float minValue, float maxValue, float defaultvalue);
	AudioParameterFloat* GetFloat(String id);

};

#endif //PARAMETER_HANDLER_H
