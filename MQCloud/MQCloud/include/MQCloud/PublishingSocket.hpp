#ifndef COREPUBLISHINGSOCKET_HPP
#define COREPUBLISHINGSOCKET_HPP

#include <memory>
#include <MQCloud/Socket.hpp>

namespace MQCloud {
    // Publish socket (one to many)
    struct PublishingSocketFactory {
        virtual std::shared_ptr<OutputSocketDecorator> CreatePublishingSocket();

        virtual ~PublishingSocketFactory(){}
    };
}
#endif // !COREPUBLISHINGSOCKET_HPP

