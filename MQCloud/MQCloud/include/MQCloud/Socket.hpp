#include <MQCloud/Delegates.hpp>

#include <memory>

#ifndef SOCKETHPP
#define SOCKETHPP

namespace MQCloud {
    struct ISocket {
        virtual std::string  GetSocketId() = 0;
		virtual void Connect() = 0;
        virtual void AddDisconnectHandler(std::shared_ptr<OnError> handler) =0;

        //Shall close socket
        virtual ~ISocket() {}
    };


	struct IInputSocket : ISocket {
		virtual void OnMessage(std::shared_ptr<MQCloud::OnMessageAction> action) =0;
	};

	struct IOutputSocket : ISocket {
		virtual void PublishMessage(const Message& msg) =0;
	};
}

}
#endif // !SOCKETHPP

