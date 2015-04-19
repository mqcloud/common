// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
#include <string>
#include "MQCloud.h"

struct String {
	String(std::string && data);

	explicit String(const std::string & data);

	operator CString();
private:
	CString native_string;
	std::string base;
};


////
struct Messsage {
	Messsage(std::string topic, std::string data) {
		message = MQCloud.MessageUtilities.GetNewMessage();
		MQCloud.MessageUtilities.SetMessageData(message, String(data));
		MQCloud.MessageUtilities.SetMessageTopic(message, String(topic));
	}

private:
	Message * message;
};
