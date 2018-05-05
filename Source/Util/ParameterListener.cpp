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

#include "ParameterListener.h"



ParameterListener::ParameterListener()
{
}


ParameterListener::~ParameterListener()
{
}

void ParameterListener::handleAsyncUpdate()
{
	__intListLock.lock();
	std::vector<std::string> eventList(__pendingEventList);
	__pendingEventList.clear();
	__intListLock.unlock();
	parametersChanged(eventList);
}

void ParameterListener::callParameterChange(std::string s)
{
	__intListLock.lock();
	__pendingEventList.push_back(s);
	__intListLock.unlock();
	triggerAsyncUpdate();
}

void ParameterListener::callParameterChange(std::vector<std::string> sList)
{
	__intListLock.lock();
	for (std::string s : sList) 
	{
		sList.push_back(s);
	}
	__intListLock.unlock();
	triggerAsyncUpdate();
}
