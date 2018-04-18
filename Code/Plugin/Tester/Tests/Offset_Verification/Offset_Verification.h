#ifndef OFFSET_VERIFICATION_H
#define OFFSET_VERIFICATION_H
#include "TestHost.h"
class Offset_Verification : public TestHost::Test {
	
	virtual const char * name() const {
		return "Offset_Verification";
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
					58, //A5
					0.0,
					1.0,
					0,
					-1

				};

				events.addEvent(e);
				e.noteOn = {
					0,
					70, //A5
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
		return true;
	}
			
	virtual bool hasParameterFile() {
		return true;
	}
};

#endif