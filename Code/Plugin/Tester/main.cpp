#include "wrapperVST.h"
#include <iostream>







int  main(){

	wrapperVST vst;

	if (!vst.isInitialized()) {
		return 42;
	}


	Steinberg::Vst::SpeakerArrangement inspeakers = Steinberg::Vst::SpeakerArr::kEmpty;
	Steinberg::Vst::SpeakerArrangement outspeakers = Steinberg::Vst::SpeakerArr::kStereo;
	
	if (vst->setBusArrangements(&inspeakers, 0, &outspeakers, 1) != Steinberg::kResultTrue) {
		return 44;
	}

	Steinberg::Vst::ProcessSetup config = {
		Steinberg::Vst::kRealtime,
		Steinberg::Vst::kSample64,
		1024,
		44100.0	
	};

	if (vst->setupProcessing(config) != Steinberg::kResultOk) {
		return 43;
	}


	vst->setProcessing(true);
	Steinberg::Vst::ProcessData data = {};
	vst->setProcessing(false);

	

	return 0;
}