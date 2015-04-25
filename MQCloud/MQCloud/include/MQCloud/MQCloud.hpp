// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches


#include <MQCloud/MQCloud.h>
#include <MQCloud/CXX/CStringAdaptor.hpp>
#include <MQCloud/CXX/Message.hpp>
#include <MQCloud/CXX/UserMessage.hpp>

#ifndef MQCloudCXX
#define MQCloudCXX

	// Publish socket (one to many)
	struct CorePublishingSocketBase : protected CorePublishingSocketInterface {
		virtual void CoreBindPublishingSocket(int socketId, const std::string & addr);
		virtual void CorePublishMessage(int socketId, const Message * msg);
		virtual ~CorePublishingSocketBase();
	};


	// Subscribe socket (one to many)
	struct CoreSubscriberSocketBase : protected CoreSubscriberSocketInterface {
		CoreSubscriberSocketBase( std::function<void(const Message * msg)> callback);
		virtual void CoreConnectSubscribingSocket(int socketId, const std::string & addr);
		virtual ~CoreSubscriberSocketBase();
	};


	// Threading (for callbacks execution and core logic)
	struct CoreThreadManagementBase : protected CoreThreadManagementInterface {
		virtual int CoreGetAllowedThreadPoolMaxSize();
		virtual int CoreCreateThread();
		virtual void CoreDeleteThread(int threadId);
		virtual void AddTask(int threadId, std::function<void()> task, std::function<void()>callback);
		virtual ~CoreThreadManagementBase();
	};

	struct CoreConfigurationBase : CoreConfiguration {
		std::unique_ptr<CoreThreadManagementBase> ThreadingInterface;
		std::unique_ptr<CoreSubscriberSocketBase> SubscriberSocketInterface;
		std::unique_ptr<CorePublishingSocketBase> PublishingSocketInterface;
	};


	struct BackEndFactoryBase : protected BackEnd {
		virtual std::shared_ptr<CoreConfigurationBase> CreateContext();
		virtual ~BackEndFactoryBase();
	};

#endif // MQCloudCXX