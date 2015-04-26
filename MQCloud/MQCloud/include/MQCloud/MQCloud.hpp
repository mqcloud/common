// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches


#include <MQCloud/MQCloud.h>
#include <MQCloud/CXX/CStringAdaptor.hpp>
#include <MQCloud/CXX/Message.hpp>
#include <MQCloud/CXX/UserMessage.hpp>

#ifndef MQCloudCXX
#define MQCloudCXX
#include <memory>

// Publish socket (one to many)
struct CorePublishingSocketBase : protected CorePublishingSocketInterface {
	CorePublishingSocketBase();
	CorePublishingSocketBase(CorePublishingSocketInterface base);
	virtual void CoreBindPublishingSocket(int socketId, const std::string & addr);
	virtual void CorePublishMessage(int socketId, const Message * msg);
	virtual ~CorePublishingSocketBase();
};


// Subscribe socket (one to many)
struct CoreSubscriberSocketBase : protected CoreSubscriberSocketInterface {
	CoreSubscriberSocketBase(CoreSubscriberSocketInterface base);
	CoreSubscriberSocketBase(std::function<void(const Message * msg)> callback);
	virtual void CoreConnectSubscribingSocket(int socketId, const std::string & addr);
	virtual ~CoreSubscriberSocketBase();
};


// Threading (for callbacks execution and core logic)
struct CoreThreadManagementBase : protected CoreThreadManagementInterface {
	CoreThreadManagementBase();
	CoreThreadManagementBase(CoreThreadManagementInterface base);
	virtual int CoreGetAllowedThreadPoolMaxSize();
	virtual int CoreCreateThread();
	virtual void CoreDeleteThread(int threadId);
	virtual void AddTask(int threadId, std::function<void()> task, std::function<void()> callback);
	virtual ~CoreThreadManagementBase();
};

struct CoreConfigurationBase : CoreConfiguration {
	CoreConfigurationBase();
	CoreConfigurationBase(CoreConfiguration base);
	std::unique_ptr<CoreThreadManagementBase> ThreadingInterface;
	std::unique_ptr<CoreSubscriberSocketBase> SubscriberSocketInterface;
	std::unique_ptr<CorePublishingSocketBase> PublishingSocketInterface;
	virtual ~CoreConfigurationBase();
};


struct BackEndFactoryBase : protected BackEnd {
	std::shared_ptr<CoreConfigurationBase> CreateCtx();
	virtual CoreConfigurationBase * CreateContext();
	virtual void FreeContext(CoreConfigurationBase *);
	virtual ~BackEndFactoryBase();
};

#endif // MQCloudCXX

