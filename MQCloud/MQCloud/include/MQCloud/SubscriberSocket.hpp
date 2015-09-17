#ifndef CORESUBSCRIBERSOCKET_HPP
#define CORESUBSCRIBERSOCKET_HPP

#include <MQCloud/Socket.hpp>
#include <memory>
#include <MQCloud/Socket.hpp>

namespace MQCloud {
	struct SocketConnection : Socket {
		
	};

    // Subscribe socket (one to many)
    struct SubscriberSocket {
        /*
    ment to be sync
    @return connection Subscriber socket
    */
        virtual std::shared_ptr<Socket> CreateSubscribingSocket() = 0;

        /*
    @return connection socket
    @param connection Subscriber socket of this node
    @param callback on accepting new message
    */
        virtual std::shared_ptr<SocketConnection> ConnectSubscribingSocket(std::shared_ptr<Socket> socket,
                                                                           const std::string &addr,
                                                                           std::shared_ptr<OnMessageAction> action) = 0;

        virtual ~SubscriberSocket(){}
    };
}
#endif // !CORESUBSCRIBERSOCKET_HPP

