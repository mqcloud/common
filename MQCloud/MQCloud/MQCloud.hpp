// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
#include <string>
#include "MQCloud.h"

// Here RAII will do CString cleaning for you!
struct String : CString {
	String(std::string && data);
	String(CString * other);

	explicit String(const std::string & data);
	operator std::string();
private:
	std::string base;
};


struct MesssageCXX {

	MesssageCXX(std::string topic, std::string data) {

	}

	std::string GetData() {

	}

private:
	int id;
	String data;
	String topic;
	String sender;
};
