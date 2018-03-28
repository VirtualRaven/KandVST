#ifndef Preformance_Idle_H
#define Preformance_Idle_H
#include "TestHost.h"
#include "Windows.h"
class Preformance_Idle : public TestHost::Test {

	virtual const char * name() const {
		return "Preformance_Idle";
	};

	virtual bool run(wrapperVST* vst,
		std::stringstream& msg,
		const TestHost::ParamsData& d) {
		HParameterChanges c;
		auto h = GetCurrentProcess();
		const size_t TestTimes = 3000;
		ULONG64 diff=0;
		for (size_t i = 0; i < TestTimes; i++) {
			ULONG64 pre = 0, post = 0;
			if (!QueryProcessCycleTime(h, &pre)) {
				return false;
			}
			this->block.process(vst, &c);
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