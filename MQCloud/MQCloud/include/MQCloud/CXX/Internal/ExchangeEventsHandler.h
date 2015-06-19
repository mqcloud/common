#ifndef EXCHANGEEVENTSHANDLER_HPP
#define EXCHANGEEVENTSHANDLER_HPP

#include <MQCloud/CXX/Internal/MessagesManager.hpp>
#include <MQCloud/CXX/GenericSignal.hpp>
#include <MQCloud/CXX/Protocol.pb.h>
#include <functional>

namespace MQCloud {
	namespace Internal {
		struct ExchangeEventsHandler : std::enable_shared_from_this<ExchangeEventsHandler>, OnMessageAction {
			// this functions can be turned into signals
			GenericSignalHandler<const std::string &> OnNodeUnavaliable;
			GenericSignalHandler<const std::string &, const std::string &> OnConnectionClosed;
			GenericSignalHandler<const std::string &, const std::string &> OnConnectionEstablished;
			GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeAdvertisedTopic;
			GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeRejectedTopic;
			GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeSubscribedToTopic;
			GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeUnsubscribedFromTopic;

			virtual void OnAction(const MQCloud::Message & m) override {
				Protocol::IncomingEvent incomingEvent;
				incomingEvent.ParseFromString(m.data);

				switch(incomingEvent.typecode()) {
					case Protocol::IncomingEventTypeOnConnectionClosed : {
						auto data = incomingEvent.onconnectionclosed();
						auto from = data.fromnode();
						auto to = data.tonode();
						OnConnectionClosed.OnAction(from, to);
						break;

					}
					case Protocol::IncomingEventTypeOnConnectionEstablished : {
						auto data = incomingEvent.onconnectionestablished();
						auto from = data.fromnode();
						auto to = data.tonode();
						OnConnectionEstablished.OnAction(from, to);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeAdvertisedTopic : {
						auto data = incomingEvent.onnodeadvertisedtopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeAdvertisedTopic.OnAction(node, pattern, topic);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeRejectedTopic : {
						auto data = incomingEvent.onnoderejectedtopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeRejectedTopic.OnAction(node, pattern, topic);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeSubscribedToTopic : {
						auto data = incomingEvent.onnodesubscribedtotopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeSubscribedToTopic.OnAction(node, pattern, topic);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeUnavaliable : {
						auto data = incomingEvent.onnodeunavaliable();
						auto node = data.node();
						OnNodeUnavaliable.OnAction(node);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeUnsubscribedFromTopic : {
						auto data = incomingEvent.onnodeunsubscribedfromtopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeUnsubscribedFromTopic.OnAction(node, pattern, topic);
						break;
					}
					default : break;
				}
			}
		};
	}
}
#endif // !EXCHANGEEVENTSHANDLER_HPP

