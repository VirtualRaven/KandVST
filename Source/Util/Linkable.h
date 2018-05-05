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

#ifndef LINKABLE_H
#define LINKABLE_H
#include "JuceHeader.h"
#include "Global.h"
#include "ParameterHandler.h"
#include "PresetManager.h"
class Linkable : private MouseListener, private ChangeListener
{
private:
	int __linkedTo;
	std::vector<AudioProcessorParameter *> __linkParameters;
	virtual void mouseUp(const MouseEvent& event) override;
public:
	Linkable(AudioProcessorParameter* param,GLOBAL*global);
	Linkable(std::vector<AudioProcessorParameter *> params,GLOBAL*global);
	~Linkable();

	bool isLinked();
	void Link(int id);
	void Unlink();
	virtual void LinkCouldHaveChanged() = 0;

	virtual void changeListenerCallback(ChangeBroadcaster * source) override;
	GLOBAL*Global;
};
#endif // !LINKABLE_H
