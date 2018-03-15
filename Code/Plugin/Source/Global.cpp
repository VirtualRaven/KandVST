#include "Global.h"
#include "ParameterHandler.h"
#include "PresetManager.h"
#include "Log.h"

GLOBAL::~GLOBAL()
{
	delete paramHandler;
	delete log;
	delete presetManager;
	paramHandler = nullptr;
	log = nullptr;
	presetManager = nullptr;
}

