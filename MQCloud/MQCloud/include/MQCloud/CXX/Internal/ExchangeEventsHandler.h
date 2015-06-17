#include <MQCloud/CXX/Internal/MessagesManager.hpp>
#include <MQCloud/CXX/Protocol.pb.h>
#include <functional>

#ifndef EXCHANGEEVENTSHANDLER_HPP
#define EXCHANGEEVENTSHANDLER_HPP

namespace MQCloud {
	namespace Internal {
				struct StaticEventsHandler : std::enable_shared_from_this<StaticEventsHandler>, OnMessageAction {
			// this functions can be turned into signals
			std::function<void (const std::string &)> OnNodeUnavaliable;
			std::function<void (const std::string &, const std::string &)> OnConnectionClosed;
			std::function<void (const std::string &, const std::string &)> OnConnectionEstablished;
			std::function<void (const std::string &, const std::string &, const std::string &)> OnNodeAdvertisedTopic;
			std::function<void (const std::string &, const std::string &, const std::string &)> OnNodeRejectedTopic;
			std::function<void (const std::string &, const std::string &, const std::string &)> OnNodeSubscribedToTopic;
			std::function<void (const std::string &, const std::string &, const std::string &)> OnNodeUnsubscribedFromTopic;

			virtual void OnAction(const MQCloud::Message & m) {
				Protocol::IncomingEvent event;
				event.ParseFromString(m.data);

				switch(event.typecode()) {
					case Protocol::IncomingEventTypeOnConnectionClosed: {
						auto data = event.onconnectionclosed();
						auto from = data.fromnode();
						auto to = data.tonode();
						OnConnectionClosed(from, to);
						break;

					}
					case Protocol::IncomingEventTypeOnConnectionEstablished: {
						auto data = event.onconnectionestablished();
						auto from = data.fromnode();
						auto to = data.tonode();
						OnConnectionEstablished(from, to);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeAdvertisedTopic: {
						auto data = event.onnodeadvertisedtopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeAdvertisedTopic(node, pattern, topic);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeRejectedTopic: {
						auto data = event.onnoderejectedtopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeRejectedTopic(node, pattern, topic);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeSubscribedToTopic: {
						auto data = event.onnodesubscribedtotopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeSubscribedToTopic(node, pattern, topic);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeUnavaliable: {
						auto data = event.onnodeunavaliable();
						auto node = data.node();
						OnNodeUnavaliable(node);
						break;
					}
					case Protocol::IncomingEventTypeOnNodeUnsubscribedFromTopic: {
						auto data = event.onnodeunsubscribedfromtopic();
						auto node = data.node();
						auto pattern = data.pattern();
						auto topic = data.topic();
						OnNodeUnsubscribedFromTopic(node, pattern, topic);
						break;
					}
					default: break;
				}
			}
		};
	}
}
#endif // !EXCHANGEEVENTSHANDLER_HPP
