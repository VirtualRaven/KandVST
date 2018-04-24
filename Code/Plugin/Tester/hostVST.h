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

#ifndef HOST_VST_H
#define HOST_VST_H
#include "vst/ivstevents.h"
#include "vst/ivstparameterchanges.h"

#include <vector>
#include <tuple>
#include <queue>
#include "wrapperVST.h"

using Steinberg::tresult;
using Steinberg::int32;
using Steinberg::uint32;
using Steinberg::TUID;

using Steinberg::Vst::Event;
using Steinberg::Vst::IParamValueQueue;
using Steinberg::Vst::ParamID;
using Steinberg::Vst::ParamValue;

class HEventList : public Steinberg::Vst::IEventList {
	uint32 __ref;
	std::vector<Event> __vec;
public:
	HEventList() : __ref(1) {}
	//IEventList
	virtual int32	PLUGIN_API getEventCount() override;
	virtual tresult PLUGIN_API getEvent(int32 index, Event &e) override;
	virtual tresult PLUGIN_API addEvent(Event &e) override;
	//FUnknown
	virtual tresult PLUGIN_API queryInterface(const TUID _iid, void **obj) override;
	virtual uint32 	PLUGIN_API addRef() override;
	virtual uint32 PLUGIN_API release() override;

};


class HParameterChanges : public Steinberg::Vst::IParameterChanges {
	uint32 __ref;
	std::vector<IParamValueQueue*> __vec;
	public:
		HParameterChanges() :  __ref(1) {}
		virtual ~HParameterChanges();
		virtual int32 	PLUGIN_API getParameterCount() override;
		virtual Steinberg::Vst::IParamValueQueue * PLUGIN_API getParameterData(int32 index) override;
		virtual IParamValueQueue * 	PLUGIN_API addParameterData(const Steinberg::Vst::ParamID &id, int32 &index) override;
		
		//FUnknown
		virtual tresult PLUGIN_API queryInterface(const TUID _iid, void **obj) override;
		virtual uint32 	PLUGIN_API addRef() override;
		virtual uint32 PLUGIN_API release() override;

};

class HParamValueQueue : public IParamValueQueue {
	typedef std::tuple<int32, ParamValue> TP;
	std::vector<TP> __vec;
	uint32 __ref;
	ParamID __id;
public :		
	HParamValueQueue(ParamID id) : __id(id), __ref(1) {}
	//IParamValueQueue
		virtual ParamID 	PLUGIN_API getParameterId() override;
		virtual int32 		PLUGIN_API getPointCount() override;
		virtual tresult 	PLUGIN_API getPoint(int32 index, int32 &sampleOffset, ParamValue &value) override;
		virtual tresult 	PLUGIN_API addPoint(int32 sampleOffset, ParamValue value, int32 &index) override;
		
		//FUnknown
		virtual tresult PLUGIN_API queryInterface(const TUID _iid, void **obj) override;
		virtual uint32 	PLUGIN_API addRef() override;
		virtual uint32 PLUGIN_API release() override;
};


using Steinberg::Vst::IParameterChanges;
using Steinberg::Vst::IEventList;


template<typename T> T**& ptrHelper(Steinberg::Vst::AudioBusBuffers& buff);





#endif //HOST_VST_H