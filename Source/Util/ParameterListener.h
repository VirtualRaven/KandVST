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

#ifndef PARAMETER_LISTENER
#define PARAMETER_LISTENER

#include "JuceHeader.h"
#include <mutex>
class ParameterListener : private AsyncUpdater
{
private:

	virtual void parametersChanged(std::vector<std::string>) = 0;
	// Inherited via AsyncUpdater
	virtual void handleAsyncUpdate() override;
	std::mutex __intListLock;
	std::vector<std::string> __pendingEventList;
public:
	ParameterListener();
	virtual ~ParameterListener();

	void callParameterChange(std::string);
	void callParameterChange(std::vector<std::string>);

};

#endif // !PARAMETER_LISTENER