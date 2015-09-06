//
// Created by Oleg Jakushkin on 29.07.15.
//

#include <MQCloud/Internal/ExchangeEventsManager.hpp>

explicit MQCloud::Internal::ExchangeEventsManager::ExchangeEventsManager(std::shared_ptr<ConnectionsHandler> ctx,
                                                                         const std::string &runningBackEndName) :
        MessagesManager(ctx),
        serverPatternEvent("_E"),
        serverTopicNodeEvent("_SE"),
        serverTopicHeartBeat("_SHB"),
        runningBackEndName(runningBackEndName),
        exchangeEventsHandler(std::make_shared<MQCloud::Internal::ExchangeEventsHandler>()) {
    ctx->AddOnMesageHandler(serverPatternEvent, serverTopicNodeEvent, exchangeEventsHandler);
    //exchangeEventsHandler
}

void MQCloud::Internal::ExchangeEventsManager::UnsubscribeEvent(const std::string &pattern, const std::string &topic) {
    auto event = Protocol::UnSubscribe();
    event.set_pattern(pattern);
    event.set_topic(topic);

    Protocol::OutgoingEvent outgoingMessage;

    outgoingMessage.set_typecode(Protocol::OutgoingEventTypeUnSubscribe);
    outgoingMessage.set_allocated_unsubscribe(&event);

    auto serialized = outgoingMessage.SerializeAsString();

    Message result;
    result.data = serialized;
    PrepareMessage(result);

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::SubscribeEvent(const std::string &pattern, const std::string &topic) {
    auto event = Protocol::Subscribe();
    event.set_pattern(pattern);
    event.set_topic(topic);

    Protocol::OutgoingEvent outgoingMessage;

    outgoingMessage.set_typecode(Protocol::OutgoingEventTypeSubscribe);
    outgoingMessage.set_allocated_subscribe(&event);

    auto serialized = outgoingMessage.SerializeAsString();

    Message result;
    result.data = serialized;
    PrepareMessage(result);

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::ReportNodeUnavaliable(const std::string &pattern, const std::string &topic, const std::string &node) {
    auto event = Protocol::ReportNodeUnavaliable();
    event.set_node(node);
    event.set_pattern(pattern);
    event.set_topic(topic);

    Protocol::OutgoingEvent outgoingMessage;

    outgoingMessage.set_typecode(Protocol::OutgoingEventTypeReportNodeUnavaliable);
    outgoingMessage.set_allocated_reportnodeunavaliable(&event);

    auto serialized = outgoingMessage.SerializeAsString();

    Message result;
    result.data = serialized;
    PrepareMessage(result);

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::RejectTopicEvent(const std::string &pattern, const std::string &topic) {
    auto event = Protocol::RejectTopic();
    event.set_pattern(pattern);
    event.set_topic(topic);

    Protocol::OutgoingEvent outgoingMessage;

    outgoingMessage.set_typecode(Protocol::OutgoingEventTypeRejectTopic);
    outgoingMessage.set_allocated_rejecttopic(&event);

    auto serialized = outgoingMessage.SerializeAsString();

    Message result;
    result.data = serialized;
    PrepareMessage(result);

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::AdvertizeTopicEvent(const std::string &pattern, const std::string &topic) {
    auto event = Protocol::AdvertizeTopic();
    event.set_pattern(pattern);
    event.set_topic(topic);

    Protocol::OutgoingEvent outgoingMessage;

    outgoingMessage.set_typecode(Protocol::OutgoingEventTypeAdvertizeTopic);
    outgoingMessage.set_allocated_advertizetopic(&event);

    auto serialized = outgoingMessage.SerializeAsString();

    Message result;
    result.data = serialized;
    PrepareMessage(result);

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::DisconnectEvent() {
    auto event = Protocol::Disconnect();

    Protocol::OutgoingEvent outgoingMessage;

    outgoingMessage.set_typecode(Protocol::OutgoingEventTypeDisconnect);
    outgoingMessage.set_allocated_disconnect(&event);

    auto serialized = outgoingMessage.SerializeAsString();

    Message result;
    result.data = serialized;
    PrepareMessage(result);

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::HeartBeatEvent() {
    auto event = Protocol::HeartBeat();

    Message result;
    result.data = event.SerializeAsString();

    result.PatternName = serverPatternEvent;
    result.topic       = serverTopicHeartBeat;

    ctx->PublishMessage(result);
}

void MQCloud::Internal::ExchangeEventsManager::PrepareMessage(Message &result) {
    result.PatternName = serverPatternEvent;
    result.topic       = serverTopicNodeEvent;
}