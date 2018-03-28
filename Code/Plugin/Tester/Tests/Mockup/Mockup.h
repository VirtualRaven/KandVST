#ifndef MOCKUP_H
#define MOCKUP_H
#include "TestHost.h"
class Mockup : public TestHost::Test {
	
	virtual const char * name() const {
		return "Mockup";
	};
	
	virtual bool run(wrapperVST* vst, 
		std::stringstream& msg, 
		const TestHost::ParamsData& d) {
				HParameterChanges c;
				HEventList events;
				//Create a single note on
				Steinberg::Vst::Event e;
				e.busIndex = 0;
				e.sampleOffset = 0;
				e.type = Steinberg::Vst::Event::kNoteOnEvent;
				e.noteOn = {
					0,
					42,
					0.0,
					1.0,
					0,
					-1

				};

				events.addEvent(e);
				return this->block.process(vst, &c,&events);
			}
			
	virtual bool exportTestData() {
		return true;
	}
			
	virtual bool hasPythonStep() {
		return false;
	}
			
	virtual bool hasParameterFile() {
		return true;
	}
};

#endif