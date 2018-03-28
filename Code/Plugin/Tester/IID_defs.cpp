
//This is to force the VST api to initialize IID
//we need this as we only use the interface headers
#define INIT_CLASS_IID
#include <vst/ivstaudioprocessor.h>
#include "vst/ivsteditcontroller.h"
#include <vst/ivsthostapplication.h>
#include <vst/ivstmessage.h>
#include "vst/ivstevents.h"
#include "vst/ivstparameterchanges.h"
