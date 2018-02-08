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

AudioParameterFloat * ParameterHandler::RegisterFloat(int iid, String id, String label, float minValue, float maxValue, float defaultvalue)
{
	std::string idString = std::to_string(iid) + std::string("_") + id.toStdString();
	if (__floatParams[idString] != nullptr)
		return __floatParams[idString];

	auto tmp = new AudioParameterFloat(id, label, minValue, maxValue, defaultvalue);
	__owner->addParameter(tmp);
	__floatParams[idString]=tmp;

	Global.log->Write(idString);
	Global.log->Write("\n");

	return tmp;
}

AudioParameterFloat * ParameterHandler::GetFloat(int iid, String id)
{
	return __floatParams[std::to_string(iid) + std::string("_") + id];
}
AudioParameterFloat * ParameterHandler::GetFloat(String id)
{
	return __floatParams[id];
}

std::map<String, AudioParameterFloat*>* ParameterHandler::GetFloats()
{
	return &__floatParams;
}
