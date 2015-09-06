//
// Created by Oleg Jakushkin on 29.07.15.
//
#include <MQCloud/Internal/ExchangeEventsHandler.hpp>

void virtual MQCloud::Internal::ExchangeEventsHandler::OnAction(const UserMessage &m) {
    Protocol::IncomingEvent incomingEvent;
    incomingEvent.ParseFromString(m.GetData());

    switch (incomingEvent.typecode()) {
        case Protocol::IncomingEventTypeOnConnectionClosed: {
            auto data = incomingEvent.onconnectionclosed();
            auto from = data.fromnode();
            auto to   = data.tonode();
            OnConnectionClosed.OnAction(from, to);
            break;

        }
        case Protocol::IncomingEventTypeOnConnectionEstablished: {
            auto data = incomingEvent.onconnectionestablished();
            auto from = data.fromnode();
            auto to   = data.tonode();
            OnConnectionEstablished.OnAction(from, to);
            break;
        }
        case Protocol::IncomingEventTypeOnNodeAdvertisedTopic: {
            auto data    = incomingEvent.onnodeadvertisedtopic();
            auto node    = data.node();
            auto pattern = data.pattern();
            auto topic   = data.topic();
            OnNodeAdvertisedTopic.OnAction(node, pattern, topic);
            break;
        }
        case Protocol::IncomingEventTypeOnNodeRejectedTopic: {
            auto data    = incomingEvent.onnoderejectedtopic();
            auto node    = data.node();
            auto pattern = data.pattern();
            auto topic   = data.topic();
            OnNodeRejectedTopic.OnAction(node, pattern, topic);
            break;
        }
        case Protocol::IncomingEventTypeOnNodeSubscribedToTopic: {
            auto data    = incomingEvent.onnodesubscribedtotopic();
            auto node    = data.node();
            auto pattern = data.pattern();
            auto topic   = data.topic();
            OnNodeSubscribedToTopic.OnAction(node, pattern, topic);
            break;
        }
        case Protocol::IncomingEventTypeOnNodeUnavaliable: {
            auto data = incomingEvent.onnodeunavaliable();
            auto node = data.node();
            OnNodeUnavaliable.OnAction(node);
            break;
        }
        case Protocol::IncomingEventTypeOnNodeUnsubscribedFromTopic: {
            auto data    = incomingEvent.onnodeunsubscribedfromtopic();
            auto node    = data.node();
            auto pattern = data.pattern();
            auto topic   = data.topic();
            OnNodeUnsubscribedFromTopic.OnAction(node, pattern, topic);
            break;
        }
        default:
            break;
    }
}