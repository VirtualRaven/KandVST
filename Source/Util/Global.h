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

#ifndef GLOBAL_H
#define GLOBAL_H
#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include <array>
#include <list>

class PresetManager;
class ParameterHandler;
class Log;
class LFO;

class GLOBAL {
public:
	PresetManager* presetManager;
	ParameterHandler* paramHandler;
	Log* log;

	//These pointers are not owned by us
	//and will be deleted elsewhere
	std::array<LFO*,2> lfos;
	
	struct MessageBoxInfo
	{
		AlertWindow::AlertIconType icon = AlertWindow::AlertIconType::NoIcon;
		String title = "Error";
		String message;
		String buttonText = "Ok";
	};

	void ShowMessageBox(MessageBoxInfo info);
	bool IsMessageBoxInQueue();
	MessageBoxInfo GetMessageBoxInfo();

	GLOBAL(AudioProcessor* owner);
	~GLOBAL();
	JUCE_LEAK_DETECTOR(GLOBAL);

private:
	std::list<MessageBoxInfo> __messageBoxQueue;
};

#endif // !GLOBAL_H
