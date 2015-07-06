#include <MQCloud/CXX/Socket.hpp>
#include <memory>


#ifndef CORESUBSCRIBERSOCKET_HPP
#define CORESUBSCRIBERSOCKET_HPP

namespace MQCloud {
	// Subscribe socket (one to many)
	struct SubscriberSocketInterface {
		/*
	ment to be sync
	@return connection Subscriber socket
	*/
		virtual std::shared_ptr<Socket> CoreCreateSubscribingSocket();

		/*
	@return connection socket
	@param connection Subscriber socket of this node
	@param callback on accepting new message 
	*/
		virtual std::shared_ptr<Socket> CoreConnectSubscribingSocket(std::shared_ptr<Socket> socket, const std::string& addr, std::shared_ptr<OnMessageAction> action);

		virtual ~SubscriberSocketInterface();
	};
}
#endif // !CORESUBSCRIBERSOCKET_HPP

