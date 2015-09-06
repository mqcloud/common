//
// Created by Oleg Jakushkin on 29.07.15.
//
#include <MQCloud/Internal/ResponseHandler.hpp>

void  MQCloud::Internal::ResponseHandler::AddHandler(const int &id, std::shared_ptr<::MQCloud::GenericAction> handler) {
    tbb::concurrent_hash_map<int, std::shared_ptr<OnUserMessageAction>>::accessor callback;

    handlers.insert(callback, id);
    callback->second = handler;
}

void virtual MQCloud::Internal::ResponseHandler::OnAction(const UserMessage &m) {
    auto id = m.GetMessageId();

    tbb::concurrent_hash_map<int, std::shared_ptr<OnUserMessageAction>>::accessor callback;
    if (handlers.find(callback ,id)) {
        callback->second->OnAction(m);
        handlers.erase(callback);
    }
}

void MQCloud::Internal::ResponseHandler::RemoveHandler(const int &id) {
    tbb::concurrent_hash_map<int, std::shared_ptr<OnUserMessageAction>>::accessor callback;

    if (handlers.find(callback ,id)) {
        handlers.erase(callback);
    }
}

void MQCloud::Internal::ResponseHandler::FreeAll() {
    handlers.clear();
}