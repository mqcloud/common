#ifndef EXCHANGEEVENTSHANDLER_HPP
#define EXCHANGEEVENTSHANDLER_HPP

#include <MQCloud/Internal/MessagesManager.hpp>
#include <MQCloud/GenericSignal.hpp>
#include <MQCloud/Protocol.pb.h>
#include <functional>

namespace MQCloud {
    namespace Internal {
        struct ExchangeEventsHandler :
                OnUserMessageAction {
            // this functions can be turned into signals
            GenericSignalHandler<const std::string &>                                           OnNodeUnavaliable;
            GenericSignalHandler<const std::string &, const std::string &>                      OnConnectionClosed;
            GenericSignalHandler<const std::string &, const std::string &>                      OnConnectionEstablished;
            GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeAdvertisedTopic;
            GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeRejectedTopic;
            GenericSignalHandler<const std::string &, const std::string &, const std::string &> OnNodeSubscribedToTopic;
            GenericSignalHandler<const std::string &,
                                 const std::string &,
                                 const std::string &>                                           OnNodeUnsubscribedFromTopic;

            virtual void OnAction(const UserMessage &m);
        };
    }
}
#endif // !EXCHANGEEVENTSHANDLER_HPP

