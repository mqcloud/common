#include <MQCloud/Internal/ConnectionsHandler.hpp>

#ifndef MESSAGESMANAGER_HPP
#define MESSAGESMANAGER_HPP

namespace MQCloud {
    namespace Internal {
        struct MessagesManager {
            std::shared_ptr<ConnectionsHandler> ctx;

            MessagesManager(std::shared_ptr<ConnectionsHandler> ctx) :
                    ctx(ctx) {
            }
        };
    }
}
#endif // !MESSAGESMANAGER_HPP

