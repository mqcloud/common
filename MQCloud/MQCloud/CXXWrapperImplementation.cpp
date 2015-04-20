#include <string>
#include "MQCloud.hpp"

inline String::String(std::string && _data): base(_data) {
	length = base.length();
	data = const_cast<char*>(base.c_str());
}

String::String(const CString * other) : base(other->data, other->length) {
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
