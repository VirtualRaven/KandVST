#include <iostream>
#include "util.h"
#include "TestHost.h"
#include <array>

#include "Tests.h"

std::string trim(const std::string& str) {
	auto pos = str.find_first_not_of(" \t");
	if (pos == std::string::npos)
		return "";
	auto end = str.find_last_not_of(" \t");
	return str.substr(pos, end - pos + 1);
}

//Tests
int  main(int argc, char** argv){

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



	if (argc == 1) {
		for (auto& t : test_list) {
			test.addTest(t);
		}
		return test.runTests() ? 0 : -1;
	}
	if (argc == 2) {
		auto command = trim(argv[1]);
		if (command == "params") {
			test.printParams(false);
			return 0;
		}
		else if (command == "Params") {
			test.printParams(true);
			return 0;
		}
		else if (command == "tests") {
			for (auto t : test_list)
				std::cout << t->name() << std::endl;
			return 0;
		}
	}
	if (argc == 3){
		auto flag = trim(argv[1]);
		auto value = trim(argv[1]);
		if (flag == "-T"){
			for (auto& t : test_list) {
				if (t->name()==value){
					test.addTest(t);
					break;
				}
			}
			return test.runTests() ? 0 : -1;
		}
		
	}
		std::cout << "Unknown command!" << std::endl << "Usage: tester [param|Params|tests|-T <test>]" << std::endl
			<< "Omitting command will run tests." << std::endl
			<< "Command param, lists plugin parameters." << std::endl
			<< "Command Param, lists pluing parameters in verbose mode." << std::endl
			<< "Command tests, lists test names." << std::endl;
		return 0;
	
	
}