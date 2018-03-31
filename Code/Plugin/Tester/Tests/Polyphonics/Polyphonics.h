#ifndef POLYPHONICS_H
#define POLYPHONICS_H
#include "TestHost.h"
class Polyphonics : public TestHost::Test {
	
	virtual const char * name() const {
		return "Polyphonics";
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
					0, //A5
					0.0,
					1.0,
					0,
					-1

				};
				for (size_t i = 0; i < 16; i++)
				{
					e.noteOn.pitch=63+i*3;
					events.addEvent(e);
				}
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