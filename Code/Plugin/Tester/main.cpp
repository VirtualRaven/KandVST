#include "wrapperVST.h"
#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include  <iomanip>
#include <wchar.h>
#define MIDI_PRE L"MIDI CC"

void printParameter(const Steinberg::Vst::ParameterInfo& i, bool verbose=false) {
	using namespace Steinberg::Vst;
	std::wcout << i.unitId << L" " << std::setfill(L'0') << std::setw(sizeof(decltype(i.id)) * 2) << std::hex << i.id << std::dec << L": " << i.title << L" ";
	
		if (i.stepCount == 0)
			std::cout << "R [0,1]"
			<<  " D: " << i.defaultNormalizedValue << std::endl;
		else if (i.stepCount == 1)
			std::cout << "On/Off D: "
			<< (((int)std::min(1.0, 2 * i.defaultNormalizedValue) == 0) ? "Off" : "On") << std::endl;
		else
			std::cout << "N [0," << i.stepCount << "] D: "
			<< ((int)std::min(1.0, i.stepCount * i.defaultNormalizedValue) == 0) << std::endl;
		using Steinberg::Vst::ParameterInfo;
		if (verbose) {
#define FLAG_STR(V,F) "\t\t" << #F << (V & F ? ": true" : ": false")
		std::cout << "\tFlags:\n"
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kCanAutomate) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsBypass) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsList) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsProgramChange) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsReadOnly) << std::endl
			<< "\t\t" << FLAG_STR(i.flags, ParameterInfo::kIsWrapAround) << std::endl;
#undef FLAG_STR
	}
}


int  main(){
	util::init_colour();
	wrapperVST vst;

	if (!vst.isInitialized()) {
		return 42;
	}
	vst.printInfo();
	vst.printClasses();

	std::vector<Steinberg::Vst::ParameterInfo> params;
	std::vector<Steinberg::Vst::ParameterInfo> midi_params;
	//Print info about parameters
	auto count = vst.edit()->getParameterCount();
	for ( decltype(count) i = 0; i < count; i++) {
		Steinberg::Vst::ParameterInfo info;
		if (vst.edit()->getParameterInfo(i, info) == Steinberg::kResultOk) {
			size_t len = std::min(sizeof(MIDI_PRE) / sizeof(wchar_t)-1, wcslen(info.title));
			if (0 != wmemcmp(info.title, MIDI_PRE, len))
				params.push_back(info);
			else
				midi_params.push_back(info);

		}
		else util::red([i] {std::cerr << "Parameter " << i << " failed to to be fetched" << std::endl; });
	}
	std::sort(params.begin(), params.end() - 1, 
		[](Steinberg::Vst::ParameterInfo a, Steinberg::Vst::ParameterInfo b) {return wcscmp(a.title, b.title) == -1; });

	util::cyan([] {std::cout << "### Parameters ###" << std::endl; });
	for (const auto& p : params)
		printParameter(p);


	Steinberg::Vst::SpeakerArrangement inspeakers = Steinberg::Vst::SpeakerArr::kEmpty;
	Steinberg::Vst::SpeakerArrangement outspeakers = Steinberg::Vst::SpeakerArr::kStereo;
	
	if (vst.proc()->setBusArrangements(&inspeakers, 0, &outspeakers, 1) != Steinberg::kResultTrue) {
		return 44;
	}

	Steinberg::Vst::ProcessSetup config = {
		Steinberg::Vst::kRealtime,
		Steinberg::Vst::kSample64,
		1024,
		44100.0	
	};

	if (vst.proc()->setupProcessing(config) != Steinberg::kResultOk) {
		return 43;
	}


	vst.proc()->setProcessing(true);
	Steinberg::Vst::ProcessData data = {};

	vst.proc()->setProcessing(false);

	

	return 0;
}