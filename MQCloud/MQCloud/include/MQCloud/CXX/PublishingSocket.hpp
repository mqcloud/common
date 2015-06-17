#ifndef COREPUBLISHINGSOCKET_HPP
#define COREPUBLISHINGSOCKET_HPP
#include <memory>
#include <MQCloud/CXX/Socket.hpp>

namespace MQCloud {
	// Publish socket (one to many)
	struct CorePublishingSocket {
		/*
	ment to be sync
	@return connection Publishing socket
	*/
		virtual std::shared_ptr<Socket> CoreCreatePublishingSocket();

		/*
	ment to be sync
	@param connection socket which shall send message
	@param message
	*/
		virtual void CorePublishMessage(std::shared_ptr<Socket> socket, const Message & msg);

		virtual ~CorePublishingSocket();
	};
}
#endif // !COREPUBLISHINGSOCKET_HPP