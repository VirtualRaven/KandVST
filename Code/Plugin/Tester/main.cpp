#include <iostream>
#include "util.h"
#include "TestHost.h"
#include <array>

//Tests
#include "Tests/Mockup/Mockup.h"
int  main(){
	//Create tests
	std::vector<TestHost::Test*> tests = {
		new Mockup
	};


	//Create manager for ptrs
	std::vector<std::unique_ptr<TestHost::Test>> owner;
	for (auto& t : tests)
		owner.emplace_back(t);

	//Initialize colour functions
	util::init_colour();


	TestHost test;
	if (!test.isInitialized()) {
		return 2;
	}

	for (auto& t : tests) {
		test.addTest(t);
	}
	
	return test.runTests();
	
}