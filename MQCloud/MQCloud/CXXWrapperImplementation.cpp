#include <string>
#include "MQCloud.hpp"

inline String::String(std::string && data): base(data) {
	native_string.length = base.length();
	native_string.data = base.c_str();
}

inline String::String(const std::string & data): base(data) {
	native_string.length = base.length();
	native_string.data = base.c_str();
}

String::operator ::CString() {
	return native_string;
}
