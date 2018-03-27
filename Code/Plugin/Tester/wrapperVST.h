#ifndef WRAPPERVST
#define WRAPPERVST
#include "vst/ivstaudioprocessor.h"
#include "vst/ivsteditcontroller.h"
#include "vst/ivstcomponent.h"

using Steinberg::Vst::IEditController;
using Steinberg::Vst::IAudioProcessor;
using Steinberg::Vst::IComponent;


class audioPlugin;

class wrapperVST {
	audioPlugin* __plug;
	public:
		wrapperVST();
		wrapperVST(const wrapperVST&) = delete;
		~wrapperVST();
		IAudioProcessor* proc();
		IEditController* edit();
		IComponent* com();
		bool isInitialized();
		void printClasses();
		void printInfo();
};





#endif // !WRAPPERVST
