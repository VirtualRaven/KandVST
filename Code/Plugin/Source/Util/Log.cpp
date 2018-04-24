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

#include "Log.h"



Log::Log(std::string filename):
	__file()
{
	__file.open(filename,std::ofstream::trunc);
}


Log::~Log()
{
	//__file.close();
}

template<typename T>
bool Log::Write(T text)
{
	if (!__file.is_open())
		return false;
	__fileLock.lock();
	__file << text;
	__file.flush();
	__fileLock.unlock();
	return true;
}

template bool Log::Write(char const *);
template bool Log::Write(String);
template bool Log::Write(std::string);
template bool Log::Write(int);
template bool Log::Write(float);
template bool Log::Write(double);