#ifndef COREPUBLISHINGSOCKET_HPP
#define COREPUBLISHINGSOCKET_HPP

#include <memory>
#include <MQCloud/Socket.hpp>

namespace MQCloud {
    // Publish socket (one to many)
    struct PublishingSocket {
        /*
    ment to be sync
    @return connection Publishing socket
    */
        virtual std::shared_ptr<Socket> CreatePublishingSocket() = 0;

        /*
    ment to be sync
    @param connection socket which shall send message
    @param message
    */
        virtual void PublishMessage(std::shared_ptr<Socket> socket, const Message &msg) = 0;

        virtual ~PublishingSocket(){}
    };
}
#endif // !COREPUBLISHINGSOCKET_HPP

