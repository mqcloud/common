#include <string>
#include <functional>
#include <MQCloud/CXX/Message.hpp>

#ifndef UserMessageHpp
#define UserMessageHpp
namespace MQCloud {
struct UserMessage : protected Message {
	void SetData(const std::string & _data);
	std::string & GetData();
	void SetTopic(const std::string & _data);
	std::string & GetTopic();
	std::string & GetSender();
};
}
#endif // UserMessageHpp

	