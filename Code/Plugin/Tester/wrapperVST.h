#ifndef WRAPPERVST
#include "vst/ivstaudioprocessor.h"

using Steinberg::Vst::IAudioProcessor;



class audioPlugin;

class wrapperVST {
	audioPlugin* __plug;
	public:
		wrapperVST();
		wrapperVST(const wrapperVST&) = delete;
		~wrapperVST();
		IAudioProcessor* operator->();
		bool isInitialized();
};


#endif // !WRAPPERVST
