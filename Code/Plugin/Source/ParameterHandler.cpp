#include "ParameterHandler.h"


ParameterHandler::ParameterHandler(AudioProcessor& owner)
{
	__owner = &owner;
	//__owner = owner;
	__floatParams = std::map<String, AudioParameterFloat*>();
	__intParams = std::map<String, AudioParameterInt*>();
	__boolParams = std::map<String, AudioParameterBool*>();
	__choiceParams = std::map<String, AudioParameterChoice*>();
}

ParameterHandler::~ParameterHandler()
{

}

AudioParameterFloat * ParameterHandler::RegisterFloat(int iid, String id, String label, float minValue, float maxValue, float defaultvalue)
{
	std::string idString = std::to_string(iid) + std::string("_") + id.toStdString();
	if (__floatParams[idString] != nullptr)
		return __floatParams[idString];

	label = label + std::string(" ") + std::to_string(iid);
	auto tmp = new AudioParameterFloat(idString, label, minValue, maxValue, defaultvalue);
	__owner->addParameter(tmp);
	__floatParams[idString]=tmp;


	return tmp;
}

AudioParameterInt * ParameterHandler::RegisterInt(int iid, String id, String label, int minValue, int maxValue, int defaultvalue)
{
	std::string idString = std::to_string(iid) + std::string("_") + id.toStdString();
	if (__intParams[idString] != nullptr)
		return __intParams[idString];

	label = label + std::string(" ") + std::to_string(iid);
	auto tmp = new AudioParameterInt(idString, label, minValue, maxValue, defaultvalue);
	__owner->addParameter(tmp);
	__intParams[idString] = tmp;
	return tmp;
}

AudioParameterBool * ParameterHandler::RegisterBool(int iid, String id, String label, bool defaultvalue)
{
	std::string idString = std::to_string(iid) + std::string("_") + id.toStdString();
	if (__boolParams[idString] != nullptr)
		return __boolParams[idString];

	label = label + std::string(" ") + std::to_string(iid);
	auto tmp = new AudioParameterBool(idString, label, defaultvalue);
	__owner->addParameter(tmp);
	__boolParams[idString] = tmp;
	return tmp;
}


AudioParameterChoice * ParameterHandler::RegisterChoice(int iid, String id, String label,StringArray &choices, int defaultItemIndex)
{
	std::string idString = std::to_string(iid) + std::string("_") + id.toStdString();
	if (__choiceParams[idString] != nullptr)
		return __choiceParams[idString];

	label = label + std::string(" ") + std::to_string(iid);
	auto tmp = new AudioParameterChoice(idString, label, choices, defaultItemIndex);
	__owner->addParameter(tmp);
	__choiceParams[idString] = tmp;
	return tmp;
}
AudioParameterFloat * ParameterHandler::GetFloat(String id)
{
	return __floatParams[id];
}

template<> AudioParameterFloat * ParameterHandler::Get(int iid, String id)
{
	return __floatParams[std::to_string(iid) + std::string("_") + id];
}
template<> AudioParameterInt * ParameterHandler::Get(int iid, String id)
{
	return __intParams[std::to_string(iid) + std::string("_") + id];
}
template<> AudioParameterBool * ParameterHandler::Get(int iid, String id)
{
	return __boolParams[std::to_string(iid) + std::string("_") + id];
}
template<> AudioParameterChoice * ParameterHandler::Get(int iid, String id)
{
	return __choiceParams[std::to_string(iid) + std::string("_") + id];
}
template<> std::map<String,AudioParameterFloat*> * ParameterHandler::GetAll()
{
	return &__floatParams;
}
template<> std::map<String, AudioParameterInt*> * ParameterHandler::GetAll()
{
	return &__intParams;
}
template<> std::map<String, AudioParameterBool*> * ParameterHandler::GetAll()
{
	return &__boolParams;
}
template<> std::map<String, AudioParameterChoice*> * ParameterHandler::GetAll()
{
	return &__choiceParams;
}