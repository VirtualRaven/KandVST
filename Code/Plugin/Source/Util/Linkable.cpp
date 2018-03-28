#include "Linkable.h"

void Linkable::mouseUp(const MouseEvent & event)
{
}

Linkable::Linkable(AudioProcessorParameter* param, GLOBAL*global)
{
	Global = global;
	__linkParameters = std::vector<AudioProcessorParameter *>();
	__linkParameters.push_back(param);
	Global->presetManager->addChangeListener(this);
	__linkedTo = -1;
	if (__linkParameters.size() != 0) {
		auto sender = Global->paramHandler->GetSender(__linkParameters[0]);
		if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (sender))
		{
			__linkedTo = stoi(pid->paramID.toStdString().substr(0, pid->paramID.toStdString().find_first_of('_')));
		}
	}
}

Linkable::Linkable(std::vector<AudioProcessorParameter *> params,GLOBAL*global)
{
	Global = global;

	__linkParameters = std::vector<AudioProcessorParameter *>(params);
	Global->presetManager->addChangeListener(this);
	__linkedTo = -1;
	if (__linkParameters.size() != 0) {
		auto sender = Global->paramHandler->GetSender(__linkParameters[0]);
		if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (sender))
		{
			__linkedTo = stoi(pid->paramID.toStdString().substr(0, pid->paramID.toStdString().find_first_of('_')));
		}
	}
}

Linkable::~Linkable()
{
}

bool Linkable::isLinked()
{
	return __linkedTo!=-1;
}

void Linkable::Link(int id)
{
	if (__linkedTo == -1) {
		for (auto p : __linkParameters)
		{
			if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (p))
			{
				std::map<int, AudioProcessorParameterWithID*> similar = Global->paramHandler->FindSimilar(pid);
				if (similar.count(id) == 1) {
					AudioProcessorParameter* rec = similar[id];

					Global->paramHandler->LinkParameters(rec, p);
				}
			}
		}
	}
	__linkedTo = id;
}

void Linkable::Unlink()
{
	if (__linkedTo != -1) {
		for (auto p : __linkParameters)
		{
			if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (p))
			{
				std::map<int, AudioProcessorParameterWithID*> similar = Global->paramHandler->FindSimilar(pid);
				if (similar.count(__linkedTo) == 1) {
					AudioProcessorParameter* rec = similar[__linkedTo];

					Global->paramHandler->RemoveLink(rec, p);
				}
			}
		}
	}
	__linkedTo = -1;
}

void Linkable::changeListenerCallback(ChangeBroadcaster * source)
{
	__linkedTo = -1;
	if (__linkParameters.size() != 0) {
		auto sender = Global->paramHandler->GetSender(__linkParameters[0]);
		if (auto* pid = dynamic_cast<AudioProcessorParameterWithID*> (sender))
		{
			__linkedTo = stoi(pid->paramID.toStdString().substr(0, pid->paramID.toStdString().find_first_of('_')));
		}
	}
	LinkCouldHaveChanged();
}
