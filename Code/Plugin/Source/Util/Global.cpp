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

