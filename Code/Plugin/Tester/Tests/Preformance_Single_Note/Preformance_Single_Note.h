#ifndef Preformance_Single_Note_H
#define Preformance_Single_Note_H
#include "TestHost.h"
#include "Windows.h"
class Preformance_Single_Note : public TestHost::Test {

	virtual const char * name() const {
		return "Preformance_Single_Note";
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

		auto h = GetCurrentProcess();
		const size_t TestTimes = 3000;
		ULONG64 diff = 0;
		for (size_t i = 0; i < TestTimes; i++) {
			ULONG64 pre = 0, post = 0;
			if (!QueryProcessCycleTime(h, &pre)) {
				return false;
			}
			this->block.process(vst, &c,&events);
			if (!QueryProcessCycleTime(h, &post)) {
				return false;
			}
			msg << "Iteration " << i << ": " << (post - pre) << std::endl;
			diff += (post - pre) / TestTimes;
		}
		msg << "Average:" << diff << std::endl;
		return true;
	}

	virtual bool exportTestData() {
		return false;
	}

	virtual bool hasPythonStep() {
		return false;
	}

	virtual bool hasParameterFile() {
		return true;
	}
};

#endif