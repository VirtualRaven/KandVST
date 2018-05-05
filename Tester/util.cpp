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

#include "util.h"
#include <Windows.h>
HANDLE ourConsole;

void util::red(std::function<void()> f) {
	if (ourConsole != NULL) {
		SetConsoleTextAttribute(ourConsole, FOREGROUND_RED);
		f();
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}
	else f();
}

void util::yellow(std::function<void()>f){
	if (ourConsole != NULL) {
		SetConsoleTextAttribute(ourConsole, FOREGROUND_RED | FOREGROUND_GREEN );
		f();
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}
	else f();
}

void util::cyan(std::function<void()> f) {
	if (ourConsole != NULL) {
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
		f();
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}
	else f();
}

void util::init_colour() {
	ourConsole = GetStdHandle(STD_ERROR_HANDLE);
	if (ourConsole != NULL)
		SetConsoleTextAttribute(ourConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

