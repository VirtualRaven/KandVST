#ifdef VST_DEBUG
#include "folders_debug.h"
#define LIB_PATH LIB_PATH_D
#elif VST_RELEASE
#include "folders_release.h"
#define LIB_PATH LIB_PATH_R 
#else
#error "No vst3 path available for current configuration"
#endif


