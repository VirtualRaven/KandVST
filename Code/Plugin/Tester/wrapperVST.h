#ifndef WRAPPERVST
#include "vst/ivstaudioprocessor.h"
#include "vst/ivsteditcontroller.h"

using Steinberg::Vst::IEditController;
using Steinberg::Vst::IAudioProcessor;



class audioPlugin;

class wrapperVST {
	audioPlugin* __plug;
	public:
		wrapperVST();
		wrapperVST(const wrapperVST&) = delete;
		~wrapperVST();
		IAudioProcessor* proc();
		IEditController* edit();
		bool isInitialized();
		void printClasses();
		void printInfo();
};





#endif // !WRAPPERVST
