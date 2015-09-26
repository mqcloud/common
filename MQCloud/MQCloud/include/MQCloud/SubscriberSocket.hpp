#ifndef CORESUBSCRIBERSOCKET_HPP
#define CORESUBSCRIBERSOCKET_HPP

#include <MQCloud/Socket.hpp>
#include <memory>
#include <MQCloud/Socket.hpp>

namespace MQCloud {
    struct SubscriberSocketFactory {
        virtual std::shared_ptr<InputSocketDecorator> ConnectSubscribingSocket(const std::string &addr,
                                                                           std::shared_ptr<OnMessageAction> action) = 0;

        virtual ~SubscriberSocketFactory(){}
    };
}
#endif // !CORESUBSCRIBERSOCKET_HPP

