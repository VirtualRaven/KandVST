/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ParameterHandler.h"


ParameterHandler::ParameterHandler(AudioProcessor& owner)
{
	__owner = &owner;
	//__owner = owner;
	__floatParams = std::map<String, AudioParameterFloat*>();
	__intParams = std::map<String, AudioParameterInt*>();
	__boolParams = std::map<String, AudioParameterBool*>();
	__choiceParams = std::map<String, AudioParameterChoice*>();

	startTimerHz(60);
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
AudioProcessorParameter * ParameterHandler::GetParameter(int iid, std::string id)
{
	return GetParameter(createParameterId(iid, id));
}
AudioProcessorParameter * ParameterHandler::GetParameter(std::string id)
{
	if (__floatParams.count(id) == 1)
	{
		return __floatParams[id];
	}
	else if (__intParams.count(id) == 1)
	{
		return __intParams[id];
	}
	else if (__boolParams.count(id) == 1)
	{
		return __boolParams[id];
	}
	else if (__choiceParams.count(id) == 1)
	{
		return __choiceParams[id];
	}
	return nullptr;
}
bool ParameterHandler::LinkParameters(AudioProcessorParameter * sender, AudioProcessorParameter * receiver)
{
	if (__parameterLinks.count(receiver) == 1)
	{
		//Circular links not allowed
		jassert(true);
		return false;
	}
	for (auto kvp : __parameterLinks)
	{
		for (auto p : kvp.second)
		{
			if (p == receiver)
			{
				//Parameter already linked
				jassert(true);
				return false;
			}
		}
	}
	if (__parameterLinks.count(sender) == 0)
		__parameterLinks[sender] = std::vector<AudioProcessorParameter*>();
	__parameterLinks[sender].push_back(receiver);
	receiver->setValue(sender->getValue());
	return true;
}
bool ParameterHandler::LinkParameters(int sender_intId, std::string sender, int receiver_intId, std::string receiver)
{
	AudioProcessorParameter * senderParam, *receiverParam;
	senderParam = GetParameter(sender_intId, sender);
	receiverParam = GetParameter(receiver_intId, receiver);

	if (senderParam != nullptr && receiverParam != nullptr)
	{
		return LinkParameters(senderParam, receiverParam);
	}
	return false;
}
bool ParameterHandler::RemoveLink(AudioProcessorParameter * sender, AudioProcessorParameter * receiver)
{
	if (__parameterLinks.count(sender) == 1)
	{
		for (auto it =  __parameterLinks[sender].begin(); it < __parameterLinks[sender].end();it++)
		{
			if (*it == receiver) {
				__parameterLinks[sender].erase(it);
				return true;
			}
		}
	}
	return false;
}
bool ParameterHandler::RemoveLink(int sender_intId, std::string sender, int receiver_intId, std::string receiver)
{
	AudioProcessorParameter * senderParam, *receiverParam;
	senderParam = GetParameter(sender_intId, sender);
	receiverParam = GetParameter(receiver_intId, receiver);

	if (senderParam != nullptr && receiverParam != nullptr)
	{
		return RemoveLink(senderParam, receiverParam);
	}
	return false;
}
void ParameterHandler::ClearLinks()
{
	__parameterLinks.clear();
}
std::map<int,AudioProcessorParameterWithID *> ParameterHandler::FindSimilar(AudioProcessorParameterWithID* ID)
{
	std::string baseId = ID->paramID.toStdString().substr(ID->paramID.toStdString().find_first_of('_')+1);
	std::map<int,AudioProcessorParameterWithID*> similar = std::map<int,AudioProcessorParameterWithID*>();

	for (auto p : __owner->getParameters())
	{
		if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (p))
		{
			if (baseId == pid->paramID.toStdString().substr(pid->paramID.toStdString().find_first_of('_') + 1) && pid->paramID != ID->paramID) {
				similar[stoi(pid->paramID.toStdString().substr(0, pid->paramID.toStdString().find_first_of('_')))] = pid;
			}
		}
	}
	return similar;
}

const std::map<AudioProcessorParameter*, std::vector<AudioProcessorParameter*>> ParameterHandler::GetLinks()
{
	return __parameterLinks;
}

AudioProcessorParameter * ParameterHandler::GetSender(AudioProcessorParameter * link)
{
	for (auto links : __parameterLinks)
	{
		for (auto p : links.second)
		{
			if (p == link)
				return links.first;
		}
	}
	return nullptr;
}

AudioParameterFloat * ParameterHandler::GetFloat(String id)
{
	return __floatParams[id];
}

void ParameterHandler::addParamaterListener(ParameterListener * listener, int intId, std::string id)
{
	std::string idString = std::to_string(intId) + std::string("_") + id;

	if (__parameterListeners.count(idString) == 0)
		__parameterListeners[idString] = std::vector<ParameterListener*>();
	__parameterListeners[idString].push_back(listener);
}

void ParameterHandler::removeParamaterListener(ParameterListener * l)
{
	for (auto& p  : __parameterListeners)
	{
		bool remove = true;
		while (remove) {
			remove = false;
			for (auto it = p.second.begin(); it < p.second.end(); it++) {
				if (*it == l) {
					p.second.erase(it);
					remove = true;
					break;
				}
			}
		}
	}
}

void ParameterHandler::addParamaterListener(ParameterListener * listener, int intId, std::vector<std::string> ids)
{
	
	for (auto s : ids)
	{
		std::string idString = std::to_string(intId) + std::string("_") + s;
		if (__parameterListeners.count(idString) == 0)
			__parameterListeners[idString] = std::vector<ParameterListener*>();

		__parameterListeners[idString].push_back(listener);
	}

}

void ParameterHandler::timerCallback()
{
	int c = 0;
	for (auto p : __owner->getParameters())
	{
		if (__changeList.count(p) == 1 && __changeList[p] != p->getValue())
			audioProcessorParameterChanged(__owner, c, p->getValue());
		__changeList[p] = p->getValue();
		c++;
	}
}

void ParameterHandler::audioProcessorParameterChanged(AudioProcessor * processor, int parameterIndex, float newValue)
{
	//Needs to be fast
	AudioProcessorParameter * param = __owner->getParameters()[parameterIndex];
	if (__parameterLinks.count(param))
	{
		float pVal = param->getValue();
		for (auto p : __parameterLinks[param])
		{
			
			p->setValue(pVal);
		}
	}
	indexChange[head] = parameterIndex;
	head = (head + 1) % INDEX_CHANGE_SIZE;
	triggerAsyncUpdate();
}


void ParameterHandler::handleAsyncUpdate()
{
	while (tail != head)
	{
		std::string id = __owner->getParameterID(indexChange[tail]).toStdString();
		std::vector<ParameterListener*> listeners = __parameterListeners[id];
		for (auto list : listeners)
		{
			if (list != nullptr)
				list->callParameterChange(id);
		}
		
		tail = (tail + 1) % INDEX_CHANGE_SIZE;
	}
}

std::string ParameterHandler::createParameterId(int intId, std::string id)
{
	return std::to_string(intId) + std::string("_")+id;
}

template<> AudioParameterFloat * ParameterHandler::Get(int iid, String id)
{
	return __floatParams[String(iid) + String("_") + id];
}
template<> AudioParameterInt * ParameterHandler::Get(int iid, String id)
{
	return __intParams[String(iid) + String("_") + id];
}
template<> AudioParameterBool * ParameterHandler::Get(int iid, String id)
{
	return __boolParams[String(iid) + String("_") + id];
}
template<> AudioParameterChoice * ParameterHandler::Get(int iid, String id)
{
	return __choiceParams[String(iid) + String("_") + id];
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