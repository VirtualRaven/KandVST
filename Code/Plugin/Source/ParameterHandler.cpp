#include "ParameterHandler.h"


ParameterHandler::ParameterHandler(AudioProcessor& owner)
{
	__owner = &owner;
	//__owner = owner;
	__floatParams = std::map<String, AudioParameterFloat*>();
}

ParameterHandler::~ParameterHandler()
{

}

AudioParameterFloat * ParameterHandler::RegisterFloat(String id, String label, float minValue, float maxValue, float defaultvalue)
{
	auto tmp = new AudioParameterFloat(id, label, minValue, maxValue, defaultvalue);
	__owner->addParameter(tmp);
	__floatParams.insert_or_assign(id,tmp);
	return tmp;
}

AudioParameterFloat * ParameterHandler::GetFloat(String id)
{
	return __floatParams[id];
}
