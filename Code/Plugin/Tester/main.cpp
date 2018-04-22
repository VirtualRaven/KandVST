/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include "util.h"
#include "TestHost.h"
#include <array>
#include <memory>
#include "Tests.h"

std::string trim(const std::string& str) {
	auto pos = str.find_first_not_of(" \t");
	if (pos == std::string::npos)
		return "";
	auto end = str.find_last_not_of(" \t");
	return str.substr(pos, end - pos + 1);
}
struct CommandDesc {
	std::string Command;
	int			NumArguments;
	std::string Signature;
};



CommandDesc pre_defCommands[] = {
	{"T",-2,"<test> <test> ... <test>"},
	{"TC",0,"Enables TeamCity mode on tester"},
	{"P",-1,"[file] Prints all registered parameters verbose"},
	{"p",-1,"[file] Prints all registered parameters"},
	{"t",0, "Prints all registered tests"},
	{"F",0, "Force test to be run" },
	{"h",0, "Prints help page" }
};

std::map<std::string,std::vector<std::string>> commands;
//Tests
int  main(int argc, char** argv){
	//Create manager for ptrs
	std::vector<std::unique_ptr<TestHost::Test>> owner;
	for (auto& t : test_list)
		owner.emplace_back(t);

	//Initialize colour functions
	util::init_colour();


	std::string last = "";
	for (size_t i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')//New Command
		{
			std::string cmd(argv[i]);
			cmd = cmd.substr(1);
			commands[cmd].clear();
			last = cmd;
		}
		else
		{
			commands[last].emplace_back(argv[i]);
		}

		
	}


	for (auto args : commands)
	{
		/*std::cout << "Command:" << std::endl << args.first << std::endl << "Arguments:" << std::endl;
		for (size_t j = 0; j < args.second.size(); j++)
		{
			std::cout << args.second[j] << std::endl;
		}*/

		bool found = false;
		for (auto cmd : pre_defCommands)
		{
			if (cmd.Command == args.first) {
				found = true;
				if (cmd.NumArguments == 0 && args.second.size() != 0) {
					std::cout << "Did not expect argument after -" << args.first << std::endl
						<< "-" << args.first << " " << cmd.Signature;
					return 1;
				}
				else if (cmd.NumArguments > 0 && args.second.size() != cmd.NumArguments) {
					std::cout << "To few or to many arguments" << std::endl
						<< "-" << args.first << " " << cmd.Signature;
					return 1;
				}
				else if (cmd.NumArguments < 0 && args.second.size() < (cmd.NumArguments*-1 - 1)) {
					std::cout << "To few arguments" << std::endl
						<< "-" << args.first << " " << cmd.Signature;
					return 1;
				}
				break;
			}
		}
		if (!found) {
			std::cout << "Undefined command: -" << args.first << std::endl;
			return 1;
		}

	}

	TestHost test;
	if (!test.isInitialized()) {
		return 2;
	}
	bool runTests = false;
	bool intImplRun = false;
	for (auto args : commands)
	{
		std::string cmd = args.first;
		if (cmd == "T") {
			test.setTestFlag(TestHost::TestFlag::Force, true);
			for (auto arg : args.second)
			{
				for (auto& t : test_list) {
					if (t->name() == arg) {
						test.addTest(t);
						runTests = true;
						break;
					}
				}

			}
			
		}
		else if (cmd == "TC") {
			test.setTestFlag(TestHost::TestFlag::PrintTCPerf, true);
		}
		else if (cmd == "P") {
			intImplRun = true;
			if (args.second.size() > 0) {
				test.printParams(true, args.second[0]);
			}
			else {
				test.printParams(true);
			}
		}
		else if (cmd == "p") {
			intImplRun = true;

			if (args.second.size() > 0) {
				test.printParams(false, args.second[0]);
			}
			else {
				test.printParams(false);
			}
		}
		else if (cmd == "t") {
			intImplRun = true;
			for (auto t : test_list)
				std::cout << t->name() << std::endl;
		}
		else if (cmd == "h") {
			intImplRun = true;
			std::cout << "Usage:" << std::endl << "Omitting command will run tests." << std::endl;
			for (auto c : pre_defCommands) {
				std::cout << "-" << c.Command << " " << c.Signature << std::endl;
			}
		}
		else if (cmd == "F") {
			test.setTestFlag(TestHost::TestFlag::Force, true);
		}
	}



	if (!intImplRun && !runTests) {
		for (auto& t : test_list) {
			test.addTest(t);
		}
		return test.runTests() ? 0 : -1;
	}else if (runTests)
		return test.runTests() ? 0 : -1;
		return 0;
	
	
}