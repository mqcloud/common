#include <string>
#include <functional>
#include <memory>
#include <MQCloud/MQCloud.hpp>

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

CStringAdaptor::CStringAdaptor(const CString * handler): handler(const_cast<CString*>(handler)) {}

const CString * CStringAdaptor::GetData() {
	return handler;
}

std::string CStringAdaptor::GetDataCopy() {
	return std::string(reinterpret_cast<char*>(const_cast<void*>(handler->data)), handler->length);
}

void CStringAdaptor::SetData(std::string && _data) {
	if(clearData) {
		clearData();
	}
	auto data_ptr = std::unique_ptr<std::string>(new std::string(_data));
	handler->data = static_cast<const void *>(data_ptr->c_str());
	handler->length = data_ptr->length();

	clearData = std::bind([](std::unique_ptr<std::string> && d) { }, std::move(data_ptr));
}

void CStringAdaptor::SetData(const std::string * _data, std::function<void()> OnDataRemoval) {
	if(clearData) {
		clearData();
	}
	clearData = OnDataRemoval;
	handler->data = static_cast<const void *>(_data->c_str());
	handler->length = _data->length();
}

void CStringAdaptor::SetData(const CString * _data, void (* OnDataRemoval)()) {
	if(clearData) {
		clearData();
	}
	handler = const_cast<CString *>(_data);
	clearData = OnDataRemoval;
}

CStringAdaptor::~CStringAdaptor() {
	if(clearData) {
		clearData();
	}
}

void UserMessage::SetData(std::string && _data) {
	return data.SetData(move(_data));
}

void UserMessage::SetData(const std::string * _data, std::function<void()> OnDataRemoval) {
	return data.SetData(_data, OnDataRemoval);
}

std::string UserMessage::GetData() {
	return data.GetDataCopy();
}

void UserMessage::SetTopic(const std::string & _data) { // copy
	SetTopic(_data);
}

void UserMessage::SetTopic(std::string && _data) { // Move
	topic.SetData(std::string(move(_data)));
}

void UserMessage::SetTopic(const std::string * _data, std::function<void()> OnDataRemoval) { // Zero-Copy
	topic.SetData(_data, OnDataRemoval);
}

std::string UserMessage::GetTopic() {
	return topic.GetDataCopy();
}
