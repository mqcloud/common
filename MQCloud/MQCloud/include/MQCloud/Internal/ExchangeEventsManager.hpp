#ifndef EXCHANGEEVENTSMANAGER_HPP
#define EXCHANGEEVENTSMANAGER_HPP

#include <MQCloud/Internal/ExchangeEventsHandler.hpp>
#include <MQCloud/Internal/ResponseHandler.hpp>
#include <MQCloud/Internal/MessagesManager.hpp>
#include <MQCloud/Internal/PatternTopicResponseHandler.hpp>
#include <MQCloud/Protocol.pb.h>
#include <memory>

namespace MQCloud {
    namespace Internal {
        struct ExchangeEventsManager :
                MessagesManager {
            const std::string serverPatternEvent;
            const std::string serverTopicNodeEvent;
            const std::string serverTopicHeartBeat;
            const std::string runningBackEndName;

            std::shared_ptr<ResponseHandler>       eventsHandler;
            std::shared_ptr<ExchangeEventsHandler> exchangeEventsHandler;

            explicit ExchangeEventsManager(std::shared_ptr<ConnectionsHandler> ctx,
                                                       const std::string &runningBackEndName);

            void UnsubscribeEvent(const std::string &pattern, const std::string &topic);

            void SubscribeEvent(const std::string &pattern, const std::string &topic);

            void ReportNodeUnavaliable(const std::string &pattern, const std::string &topic, const std::string &node);

            void RejectTopicEvent(const std::string &pattern, const std::string &topic);

            void AdvertizeTopicEvent(const std::string &pattern, const std::string &topic);

            void DisconnectEvent();

            void HeartBeatEvent();

        private:
            void PrepareMessage(Message &result);
        };
    }
}
#endif // !EXCHANGEEVENTSMANAGER_HPP

