#ifndef HIGHPASS_FILTER_H
#define HIGHPASS_FILTER_H
#include "TestHost.h"
class Highpass_filter : public TestHost::Test {
	
	virtual const char * name() const {
		return "Highpass_filter";
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
					33, //A5 55Hz (should not be found)
					0.0,
					1.0,
					0,
					-1

				};
				events.addEvent(e);
				e.noteOn.pitch = 127; // 12543 Hz 
				events.addEvent(e);
				e.noteOn.pitch = 69; // 440 Hz
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
// https://www.midikits.net/midi_analyser/midi_note_frequency.htm