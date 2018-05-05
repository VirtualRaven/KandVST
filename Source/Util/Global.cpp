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

#include "Global.h"
#include "ParameterHandler.h"
#include "PresetManager.h"
#include "Log.h"

GLOBAL::GLOBAL(AudioProcessor* owner)
{
	presetManager = new PresetManager(owner, this);
	paramHandler = new ParameterHandler(*owner);
	log = new Log("log.txt");
}

GLOBAL::~GLOBAL()
{
	delete paramHandler;
	delete log;
	delete presetManager;
	paramHandler = nullptr;
	log = nullptr;
	presetManager = nullptr;
}

