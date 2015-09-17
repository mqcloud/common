#include <MQCloud/Delegates.hpp>

#include <memory>

#ifndef SOCKETHPP
#define SOCKETHPP

namespace MQCloud {
    struct Socket {
        std::string  SocketId;
        virtual void Connect() = 0;
        virtual void AddDisconnectHandler(std::shared_ptr<OnError> handler) =0;

        //Shall close socket
        virtual ~Socket() {}
    };

}

}
#endif // !SOCKETHPP

