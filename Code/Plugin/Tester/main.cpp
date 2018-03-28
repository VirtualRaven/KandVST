#include <iostream>
#include "util.h"
#include "TestHost.h"
#include <array>

#include "Tests.h"
//Tests
int  main(){


	//Create manager for ptrs
	std::vector<std::unique_ptr<TestHost::Test>> owner;
	for (auto& t : test_list)
		owner.emplace_back(t);

	//Initialize colour functions
	util::init_colour();


	TestHost test;
	if (!test.isInitialized()) {
		return 2;
	}

	for (auto& t : test_list) {
		test.addTest(t);
	}
	
	return test.runTests();
	
}