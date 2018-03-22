//This is to force the VST api to initialize IID
//we need this as we only use the interface headers
#define INIT_CLASS_IID

#include <Windows.h>
#include "wrapperVST.h"
#include <iostream>
#include <string>
#include "base/ipluginbase.h"
#include "base/funknown.h"
#include "folders.h"
#include <vst/ivstaudioprocessor.h>
#include <vst/ivsthostapplication.h>
#include <vst/ivstmessage.h>
#include <functional>
#include <vector>
#include "util.h"
#define APP_NAME u"AUTOMATIC TESTER"

using util::red;

struct STUID {
	Steinberg::TUID id;

	STUID(Steinberg::TUID __id) {
		memcpy(id, __id, sizeof(Steinberg::TUID));
	}
};


using Steinberg::TUID;

class TesterHost : public Steinberg::Vst::IHostApplication {
	size_t __refs;
public:
	TesterHost() : __refs(1) {}
	virtual Steinberg::tresult PLUGIN_API getName(Steinberg::Vst::String128  name) override {
		memcpy(name, APP_NAME, sizeof(APP_NAME));
		return Steinberg::kResultOk;
	}
	virtual Steinberg::tresult PLUGIN_API createInstance(TUID cid, TUID _iid, void ** obj) {
		return Steinberg::kResultFalse;
	}

	virtual Steinberg::tresult PLUGIN_API queryInterface(const TUID _iid, void **obj) override {
		QUERY_INTERFACE(_iid, obj, Steinberg::FUnknown::iid, Steinberg::FUnknown)
			QUERY_INTERFACE(_iid, obj, Steinberg::Vst::IHostApplication::iid, Steinberg::Vst::IHostApplication);
		*obj = nullptr;
		return Steinberg::kNoInterface;
	}

	virtual Steinberg::uint32 PLUGIN_API addRef() override {
		return ++__refs;
	}
	virtual Steinberg::uint32 PLUGIN_API release() override {
		return __refs = __refs > 0 ? --__refs : 0;
	}
};



class WinDLL {
	HMODULE __hdll;
	DWORD __err_code;
public:
	WinDLL(LPCTSTR file) : __hdll(LoadLibrary(file)) {
		__err_code = (__hdll == NULL) ? GetLastError() : 0;
		if (isLoaded()) {
			std::cout << "Loaded DLL: " << LIB_PATH << std::endl;
		}
	}
	WinDLL(const WinDLL&) = delete;

	~WinDLL() {
		FreeLibrary(__hdll);
	}
	bool isLoaded() {
		return __hdll != NULL;
	}

	FARPROC getPtr(LPCSTR name) {
		if (isLoaded()) {
			auto tmp = GetProcAddress(__hdll, name);
			if (tmp == NULL) {
				__err_code = GetLastError();
			}
			else {
				return tmp;
			}
		}
		return nullptr;
	}

	DWORD getErrCode() {
		return __err_code;
	}


};

class factory_plugin {
	WinDLL __dll;
	typedef bool(*DLL_FUNC)();
	DLL_FUNC __init;
	DLL_FUNC __exit;
protected:
	Steinberg::IPluginFactory* __factory;
	bool __init_success;
	std::vector<STUID> __ids;

public:
	factory_plugin() : __dll(LIB_PATH),
		__init(nullptr),
		__exit(nullptr),
		__factory(nullptr),
		__init_success(false) {
		if (__dll.isLoaded()) {

			__init = reinterpret_cast<DLL_FUNC>(__dll.getPtr("InitDll"));
			if (__init == nullptr) {
				red([] {std::cerr << "Failed to find symbol InitDll" << std::endl; });
				return;
			}

			__exit = reinterpret_cast<DLL_FUNC>(__dll.getPtr("ExitDll"));
			if (__exit == nullptr) {
				red([] {std::cerr << "Failed to find symbol ExitDll" << std::endl; });
				return;
			}

			if (__init()) {
				GetFactoryProc fproc = reinterpret_cast<GetFactoryProc>(__dll.getPtr("GetPluginFactory"));
				__factory = fproc ? fproc() : nullptr;
				if (__factory != nullptr) {
					__init_success = true;
					for (Steinberg::int32 i = 0; i < __factory->countClasses(); i++) {
						Steinberg::PClassInfo info;
						__factory->getClassInfo(i, &info);
						__ids.emplace_back(info.cid);
					}
				}
			}

		}
		else
			red([] {std::cerr << "Failed to load plugin " << LIB_PATH << std::endl; });
	}
	~factory_plugin() {
		if (__factory != nullptr)
			__factory->release();
		if (__exit != nullptr)
			__exit();
	}

	bool isInitialized() {
		return __init_success;
	}

	void printClasses() {
		util::cyan([] {std::cout << "### Classes ###" << std::endl; });
		for (Steinberg::int32 i = 0; i < __factory->countClasses(); i++) {
			Steinberg::PClassInfo info;
			__factory->getClassInfo(i, &info);
			std::cout << i << "\n\tCID: " << std::hex << info.cid << std::dec << "\n\tName: " << std::string(info.name) << "\n\tCategory: " << std::string(info.category) << std::endl << std::endl;
		}
		std::cout << std::endl;
	}

	template<typename T> T* findInterface() {
		for (const auto& id : __ids) {
			T* obj = nullptr;
			Steinberg::tresult res = this->__factory->createInstance(id.id,
				T::iid, (void**)(&obj));
			if (res == Steinberg::kResultOk && obj != nullptr)
				return obj;
			else continue;
		}
		return nullptr;
	}

	void printInfo() {
		using Steinberg::PFactoryInfo;
		Steinberg::PFactoryInfo info;
		__factory->getFactoryInfo(&info);
		util::cyan([] {std::cout << "### Plugin info ###" << std::endl; });
		std::cout << "Vendor:" << info.vendor << std::endl;
		std::cout << "Email: " << info.email << std::endl;
		std::cout << "Flags: \n"
			<< "\tkClassesDiscardable:" << (PFactoryInfo::kClassesDiscardable & info.flags ? "true" : "false") << std::endl
			<< "\tkLicenseCheck:" << (PFactoryInfo::kLicenseCheck & info.flags ? "true" : "false") << std::endl
			<< "\tkComponentNonDiscardable:" << (PFactoryInfo::kComponentNonDiscardable & info.flags ? "true" : "false") << std::endl
			<< "\tkUnicode:" << (PFactoryInfo::kUnicode & info.flags ? "true" : "false") << std::endl;
		std::cout << std::endl;

	}
};

using Steinberg::Vst::IAudioProcessor;
using Steinberg::Vst::IComponent;
using Steinberg::Vst::IAudioProcessor;
using Steinberg::Vst::IConnectionPoint;
using Steinberg::Vst::IMessage;

using Steinberg::tresult;
class ConnectionProxy : public IConnectionPoint {
	IConnectionPoint *__src, *__dest;
	Steinberg::uint32 __ref;
public:
	ConnectionProxy(IConnectionPoint* src) : __src(src), __dest(nullptr), __ref(1) {
	}
	virtual tresult PLUGIN_API connect(IConnectionPoint *other) override {
		if (__dest) {
			return Steinberg::kResultFalse;
		}
		Steinberg::tresult tmp;
		if ((tmp = __src->connect(this)) == Steinberg::kResultOk) {
			__dest = other;
		}
		return tmp;
			
	}
	virtual tresult PLUGIN_API disconnect(IConnectionPoint *other) override {
		if (other == __src) {
			auto res= __src->disconnect(this);
			__dest = nullptr;
			return res;
		}
		else return Steinberg::kInternalError;
	}

	void disconnect() {
		this->disconnect(__src);
	}

	virtual tresult PLUGIN_API notify(IMessage* message) {
		if (__dest)
			return __dest->notify(message);
		else
			return Steinberg::kInternalError;
	}
	virtual Steinberg::tresult PLUGIN_API queryInterface(const TUID _iid, void **obj) override {
			QUERY_INTERFACE(_iid, obj, Steinberg::FUnknown::iid, Steinberg::FUnknown)
			QUERY_INTERFACE(_iid, obj, Steinberg::Vst::IConnectionPoint::iid, Steinberg::Vst::IConnectionPoint)
		*obj = nullptr;
		return Steinberg::kNoInterface;
	}
	virtual Steinberg::uint32 PLUGIN_API addRef() override{
		return ++__ref;
	}
	virtual Steinberg::uint32 PLUGIN_API release() override {
		return __ref == 0 ? 0 : --__ref;
	}
	
};

class audioPlugin : public factory_plugin {
	IAudioProcessor* __pro;
	IComponent* __com;
	IEditController* __cont;
	TesterHost __host;
	
	ConnectionProxy *__c_to_p, *__p_to_c;
	IConnectionPoint* cptr;
	IConnectionPoint* pptr;
public:
	audioPlugin() : __pro(nullptr),__com(nullptr), __cont(nullptr), __c_to_p(nullptr), __p_to_c(nullptr),cptr(nullptr),pptr(nullptr) {
		if (this->isInitialized()) {

			//Get the IComponent ptr
			__com = this->findInterface<IComponent>();
			if (__com == nullptr) {
				__init_success = false;
				red([] { std::cerr << "Plugin does not implement the IComponent interface" << std::endl; });
			}

			//Initialize the component
			if (__com->initialize(static_cast<Steinberg::FUnknown*>(&__host)) != Steinberg::kResultOk) {
				//It failed and __com has been released
				__com = nullptr;
				__init_success = false;
				red([] { std::cerr << "Failed to initialize plugin IComponent" << std::endl; });
			}

			//Also get the pointer to the IAudioProcessor, wich should be implemented by the same object
			if (this->__com->queryInterface(IAudioProcessor::iid, (void**)&__pro) != Steinberg::kResultOk) {
				__init_success = false;
				red([] { std::cerr << "Plugin IComponent does not implement IAudioProcessor" << std::endl; });
			}
			
			//Check if Component also is the controller
			if (this->__com->queryInterface(IEditController::iid, (void**)&__cont) == Steinberg::kNoInterface) {
				
				Steinberg::TUID cCID;
				if ((__com->getControllerClassId(cCID) ==Steinberg::kResultOk)) {
					
					if (this->__factory->createInstance(cCID, IEditController::iid, (void**)&__cont) == Steinberg::kResultOk) {
						 
						if ( this->__cont->initialize(static_cast<Steinberg::FUnknown*>(&__host)) != Steinberg::kResultOk) {
							__init_success = false;
							red([] { std::cerr << "Failed to initialize plugin IEditController" << std::endl; });
						}

					}
					else {
						__init_success = false;
						red([] { std::cerr << "Could not create IEditController interface" << std::endl; });
					}

					__init_success = connectComponents();
				}
				else {
					__init_success = false;
					red([] { std::cerr << "Could not fetch IEditController interface cid" << std::endl; });
				}

			}
			else std::cout << "Plugin combines IComponent and IEditController" << std::endl;

		}
	}

	IAudioProcessor* proc() {
		return __pro;
	}

	IEditController *edit() {
		return __cont;
	}

	bool connectComponents() {
	
		disconnectComponents();
		if (__cont->queryInterface(IConnectionPoint::iid, (void**)&cptr) != Steinberg::kResultOk ) {
			red([] { std::cerr << "Could not fetch IConnectionPoint from controller" << std::endl; });
			return false;
		}
		if (__pro->queryInterface(IConnectionPoint::iid, (void**)&pptr) != Steinberg::kResultOk ) {
			red([] { std::cerr << "Could not fetch IConnectionPoint from processor" << std::endl; });
			return false;
		}

		cptr->connect(pptr);
		pptr->connect(cptr);

		/*
		__p_to_c = new ConnectionProxy(pptr);
		__c_to_p = new ConnectionProxy(cptr);

		if(__p_to_c->connect(cptr) != Steinberg::kResultOk ){
			red([] { std::cerr << "Could not connect processsor to controller" << std::endl; });
			return false;
		}

		if (__c_to_p->connect(pptr) != Steinberg::kResultOk) {
			red([] { std::cerr << "Could not connect controller to processor" << std::endl; });
			return false;
		}
		*/

		return true;
	}

	void disconnectComponents() {
		if (this->cptr && this->pptr) {
			cptr->disconnect(pptr);
			pptr->disconnect(cptr);
			/*
			if (this->__p_to_c) {
				this->__p_to_c->disconnect();
				delete this->__p_to_c;
				__p_to_c = nullptr;
			}
			if (this->__c_to_p) {
				this->__c_to_p->disconnect();
				delete this->__c_to_p;
				__c_to_p == nullptr;
			}*/
		}

		if (cptr) {
			cptr->release();
			cptr = nullptr;
		}
		if (pptr){
			pptr->release();
			pptr = nullptr;
		}
	}
	

	virtual ~audioPlugin() {
		
		this->disconnectComponents();
		//Order matters!!
		//IEditController must be destroyed first.
		if (__cont != nullptr) {
			__cont->terminate();
			__cont->release();
			__cont = nullptr;
		}

		if (__pro != nullptr) {
			__pro->release();
			__pro = nullptr;
		}

		if (__com != nullptr) {
			__com->terminate();
			__com->release();
			__com = nullptr;
		}
	
	
	}
};



IAudioProcessor* wrapperVST::proc() {
	return __plug->proc();
}
IEditController* wrapperVST::edit() {
	return __plug->edit();
}

bool wrapperVST::isInitialized() {
	return __plug->isInitialized();
}

wrapperVST::wrapperVST() : __plug(new audioPlugin()) {

}

void wrapperVST::printInfo() {
	this->__plug->printInfo();
}
void wrapperVST::printClasses() {
	this->__plug->printClasses();
}

wrapperVST::~wrapperVST() {
	delete __plug;
	__plug = nullptr;
}



