// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches

#include <string>
#include <utility>
#include "MQCloud.h"
#include <cassert>
#include <functional>

// Here RAII will do CString cleaning for you!
// Created mainly to 
struct String : CString {
	String(std::string && data);
	String(const CString * other);
	explicit String(const std::string & data);

	operator std::string();
	const std::string * Get();
private:
	std::string base;
};

struct CStringAdaptor {

	explicit CStringAdaptor(const CString * handler)
		: handler(const_cast<CString*>(handler)) {
	}


	const CString * GetData() {
		return handler;
	}

	std::string GetDataCopy() {
		return std::string(reinterpret_cast<char*>(const_cast<void*>(handler->data)), handler->length);
	}

	void SetData(std::string && _data) {
		if(clearData) {
			clearData();
		}
		const auto data_ptr = &_data;

		std::function<void()> OnDataRemoval = std::bind(
		                                                [](std::string && d) {
		                                                },
		                                                std::move(_data)
		                                               );

		SetData(data_ptr, OnDataRemoval);
	}

	void SetData(const std::string * _data, std::function<void ()> OnDataRemoval) {
		if(clearData) {
			clearData();
		}
		clearData = OnDataRemoval;
		handler->data = static_cast<const void *>(_data->c_str());
		handler->length = _data->length();
	}


	void SetData(const CString * _data, void (*OnDataRemoval)()) {
		if(clearData) {
			clearData();
		}
		clearData = OnDataRemoval;
		handler = const_cast<CString *>(_data);
	}

	~CStringAdaptor() {
		if(clearData) {
			clearData();
		}
	}

private:
	CString * handler; // ReadOnly!
	std::function<void ()> clearData;
};

struct Message : protected CoreMessage {
	CStringAdaptor topic;
	CStringAdaptor data;
	CStringAdaptor serviceId;
	CStringAdaptor PatternName;
	CStringAdaptor PatternData;
	Message();
	int GetMessageId();
	void SetMessageId(const int &);

private:
	std::function<void ()> clearTopic;
};
