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
		const size_t TestTimes = 100;
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