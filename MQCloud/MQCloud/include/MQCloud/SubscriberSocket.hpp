#ifndef CORESUBSCRIBERSOCKET_HPP
#define CORESUBSCRIBERSOCKET_HPP

#include <MQCloud/Socket.hpp>
#include <memory>
#include <MQCloud/Socket.hpp>

namespace MQCloud {
    // Subscribe socket (one to many)
    struct SubscriberSocketInterface {
        /*
    ment to be sync
    @return connection Subscriber socket
    */
        virtual std::shared_ptr<Socket> CreateSubscribingSocket();

        /*
    @return connection socket
    @param connection Subscriber socket of this node
    @param callback on accepting new message
    */
        virtual std::shared_ptr<Socket> ConnectSubscribingSocket(std::shared_ptr<Socket> socket,
                                                                           const std::string &addr,
                                                                           std::shared_ptr<OnMessageAction> action);

        virtual ~SubscriberSocketInterface();
    };
}
#endif // !CORESUBSCRIBERSOCKET_HPP

