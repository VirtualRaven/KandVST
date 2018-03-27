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

