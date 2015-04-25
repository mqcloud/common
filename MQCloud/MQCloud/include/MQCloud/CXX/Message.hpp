#include <functional>
#include <MQCloud/CXX/CStringAdaptor.hpp>

#ifndef MessageHpp
#define MessageHpp

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

#endif // MessageHpp

