#include <string>
#include "MQCloud.hpp"

inline String::String(std::string && _data): base(_data) {
	length = base.length();
	data = const_cast<char*>(base.c_str());
}

String::String(const CString * other) : base(static_cast<const char *>(other->data), other->length) {
	length = base.length();
	data = const_cast<char*>(base.c_str());
}

inline String::String(const std::string & _data): base(_data) {
	length = base.length();
	data = const_cast<char*>(base.c_str());
}

String::operator std::basic_string<char>() {
	return base;
}

const std::string * String::Get() {
	return & base;
}

inline Message::Message(): topic(topic),
                           data(data),
                           serviceId(serviceId),
                           PatternName(PatternName),
                           PatternData(PatternData) {
	static auto id = 0;
	messaageId = id++;
}

int ::Message::GetMessageId() {
	return messaageId;
}

void ::Message::SetMessageId(const int & id) {
	messaageId = id;
}
