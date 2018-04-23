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
