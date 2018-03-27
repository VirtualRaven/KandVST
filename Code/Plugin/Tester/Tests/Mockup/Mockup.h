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
				return this->block.process(vst, &c);
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