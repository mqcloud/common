// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
#include <string>
#include <utility>
#include "MQCloud.h"
#include <memory>

// Here RAII will do CString cleaning for you!
struct String : CString {
	String(std::string && data);
	String(const CString * other);

	explicit String(const std::string & data);
	operator std::string();
private:
	std::string base;
};

struct CMesssage : CoreMessage {
	CMesssage() {
		static int _id = 0;
		messaageId = _id++;
	}

	void SetData(const CString * _data, void (*OnTopicRemoval)()) {
		if(clearData) {
			clearData();
		}
		clearData =OnTopicRemoval;
		data = *_data;
	}

	void SetTopic(const CString * _topic, void (*OnTopicRemoval)()) {
		if(clearTopic) {
			clearTopic();
		}
		clearTopic =OnTopicRemoval;
		topic = *_topic;
	}

	~CMesssage() {
		if(clearData) {
			clearData();
		}
		if(clearTopic) {
			clearTopic();
		}
	}
private:
	void (* clearData)();
	void (* clearTopic)();
};

struct CxxMesssageAdaptor {
	
	std::string _data;
	std::string _topic;

	CxxMesssageAdaptor() {
		m = MQCloud.MessageUtilities.GetNewMessage();
		messaageId = MQCloud.MessageUtilities.GetMessageId(m);
		_serviceId = String(MQCloud.MessageUtilities.GetMessageSenderServiceId(m));
	}

	int GetMessaageId() {
		return messaageId;
	}

	int GetServiceId() {
	}

	operator Message*(){
		//std::unique_ptr<String> d{new String(_data)};
		String d(_data);
		String t(_topic);
		MQCloud.MessageUtilities.SetMessageTopic(m, &t, [](){}); // And here we get a memory leak...
		

		return m;
	}

private:
	int messaageId;
	std::string _serviceId;
	Message * m;
};
