#include <MQCloud/CXX/Delegates.hpp>

#include <memory>

#ifndef SOCKETHPP
#define SOCKETHPP
namespace MQCloud {
	struct Socket {
		std::string SocketId;
		virtual void AddDisconnectHandler(std::shared_ptr<OnError> handler);

		//Shall close socket
		virtual ~Socket() {}
	}; 
}

}
#endif // !SOCKETHPP
