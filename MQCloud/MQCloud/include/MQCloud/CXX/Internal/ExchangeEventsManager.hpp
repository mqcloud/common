#ifndef EXCHANGEEVENTSMANAGER_HPP
#define EXCHANGEEVENTSMANAGER_HPP

#include <MQCloud/CXX/Internal/ExchangeEventsHandler.h>
#include <MQCloud/CXX/Internal/ResponseHandler.hpp>
#include <MQCloud/CXX/Internal/MessagesManager.hpp>
#include <MQCloud/CXX/Internal/PatternTopicResponseHandler.hpp>
#include <MQCloud/CXX/Protocol.pb.h>
#include <memory>

namespace MQCloud {
	namespace Internal {
		struct ExchangeEventsManager : MessagesManager {
			// _SE
			const std::string serverPatternEvent;

			// _SN
			const std::string serverTopicNodeEvent;
			const std::string serverTopicHeartBeat;

			const std::string runningBackEndName;

			std::shared_ptr<ResponseHandler> eventsHandler;
			std::shared_ptr<ExchangeEventsHandler> exchangeEventsHandler;

			explicit ExchangeEventsManager(std::shared_ptr<ConnectionsHandler> ctx, const std::string & runningBackEndName)
				: MessagesManager(ctx), serverPatternEvent("_E"), serverTopicNodeEvent("_SE"), serverTopicHeartBeat("_SHB"), runningBackEndName(runningBackEndName), exchangeEventsHandler(std::make_shared<ExchangeEventsHandler>) {
				ctx->AddOnMesageHandler(serverPatternEvent, serverTopicNodeEvent, exchangeEventsHandler);
				//exchangeEventsHandler
			}

			void UnsubscribeEvent(const std::string & pattern, const std::string & topic) {
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

			void SubscribeEvent(const std::string & pattern, const std::string & topic) {
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

			void ReportNodeUnavaliable(const std::string & pattern, const std::string & topic, const std::string & node) {
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

			void RejectTopicEvent(const std::string & pattern, const std::string & topic) {
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

			void AdvertizeTopicEvent(const std::string & pattern, const std::string & topic) {
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

			void DisconnectEvent() {
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

			void HeartBeatEvent() {
				auto event = Protocol::HeartBeat();

				Message result;
				result.data = event.SerializeAsString();

				result.PatternName = serverPatternEvent;
				result.topic = serverTopicHeartBeat;

				ctx->PublishMessage(result);
			}

		private:
			void PrepareMessage(Message & result) {
				result.PatternName = serverPatternEvent;
				result.topic = serverTopicNodeEvent;
			}
		};
	}
}
#endif // !EXCHANGEEVENTSMANAGER_HPP

