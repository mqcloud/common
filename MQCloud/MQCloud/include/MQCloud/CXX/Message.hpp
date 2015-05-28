#include <functional>
#include <MQCloud/CXX/CStringAdaptor.hpp>

#ifndef MessageHpp
#define MessageHpp

namespace MQCloud {
	struct Message {
		std::string topic;
		std::string data;
		std::string serviceId;
		std::string PatternName;
		std::string PatternData;
		Message();
		Message(const std::string & data, const std::string & topic, const std::string & pattern, const std::string & patternData);
		int GetMessageId();
		void SetMessageId(const int &);
	
	private:
		std::function<void ()> clearTopic;
	};
}
#endif // MessageHpp

