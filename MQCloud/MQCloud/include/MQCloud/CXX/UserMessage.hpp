#include <string>
#include <functional>
#include <MQCloud/CXX/Message.hpp>

#ifndef UserMessageHpp
#define UserMessageHpp

struct UserMessage : protected Message {
	void SetData(std::string && _data);
	void SetData(const std::string * _data, std::function<void ()> OnDataRemoval);
	std::string GetData();
	void SetTopic(const std::string & _data); // copy
	void SetTopic(std::string && _data);// Move
	void SetTopic(const std::string * _data, std::function<void ()> OnDataRemoval); // Zero-Copy
	std::string GetTopic();
};

#endif // UserMessageHpp

