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
struct CorePublishingSocketBase {
	CorePublishingSocketBase();
	virtual void CoreBindPublishingSocket(int socketId, const std::string & addr);
	virtual void CorePublishMessage(int socketId, const Message * msg);
	virtual ~CorePublishingSocketBase();
};

// Subscribe socket (one to many)
struct CoreSubscriberSocketBase {
	CoreSubscriberSocketBase(std::function<void(const Message * msg)> callback);
	virtual void CoreConnectSubscribingSocket(int socketId, const std::string & addr);
	virtual ~CoreSubscriberSocketBase();
};


// Threading (for callbacks execution and core logic)
struct CoreThreadManagementBase {
	CoreThreadManagementBase();
	virtual int CoreGetAllowedThreadPoolMaxSize();
	virtual int CoreCreateThread();
	virtual void CoreDeleteThread(int threadId);
	virtual void AddTask(int threadId, std::function<void()> task, std::function<void()> callback);
	virtual ~CoreThreadManagementBase();
};

struct CoreConfigurationBase {
	std::unique_ptr<CoreThreadManagementBase> ThreadingInterface;
	std::unique_ptr<CoreSubscriberSocketBase> SubscriberSocketInterface;
	std::unique_ptr<CorePublishingSocketBase> PublishingSocketInterface;

	virtual ~CoreConfigurationBase() {}
};

struct EventsHandlerBase {
	virtual void OnSendError(int errorCode, int messageId);
	virtual void OnReadError(int errorCode);
	virtual void OnExchangeError(int errorCode);
	virtual void OnConnectedToExchange();
	virtual void OnServiceRegistred(const std::string & uid);
	virtual void OnNewSubscriber(const std::string & topic, const std::string & subscriber);

	virtual ~EventsHandlerBase() {}
};

struct BackEndFactoryBase : protected BackEnd {
	std::string BackEndName;
	virtual CoreConfigurationBase * CreateContext();
	virtual void FreeContext(CoreConfigurationBase * ctx);

	virtual ~BackEndFactoryBase() {}
};

struct ExtensiabiletyEventsHandlerBase {
	virtual void OnNodeConnectedToOther(const std::string & node);
	virtual void OnNodeConnectedToThis(const std::string & node);
	virtual void OnMessage(const Message * in);
	virtual void OnAdvertisedTopic(const std::string & pattern, const std::string & topic, const std::string & node);
	virtual void OnNodeRejectedTopic(const std::string & pattern, const std::string & topic, const std::string & node);

	virtual ~ExtensiabiletyEventsHandlerBase() {}
};

struct FrontEndBase {
	virtual int AddExtensiabiletyEventsHandler(const CoreConfiguration * ctx, ExtensiabiletyEventsHandler * handler);
	virtual void RemoveExtensiabiletyEventsHandler(const CoreConfiguration * ctx, int handlerId);

	virtual void AdvertiseTopic(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, std::function<void(const Message *)> OnMessage);
	virtual void RejectTopic(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic);

	virtual void Subscribe(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnSubscribed)(const ServiceId * nodes, int count), void (*OnMessage)(const Message * in));
	virtual void Unsubscribe(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic);

	virtual void PublishMessageToAnyNode(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, const CoreMessage * out);
	virtual void PublishMessageToAllNodes(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, const CoreMessage * out);
	virtual void PublishMessageToNode(const CoreConfiguration * ctx, const ServiceId * node, const CoreMessage * out);
	virtual void PublishMessageToNodes(const CoreConfiguration * ctx, const ServiceId * nodes, int nodesCount, const CoreMessage * out);

	virtual void GetAllSubscribedNodes(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));
	virtual void GetAllPublishingNodes(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, void (*OnResult)(const ServiceId * nodes, int count));

	//Load balancing
	int SetGeneralNodeIdSelectionAlgorithm(const CoreConfiguration * ctx, const ServiceId * (*algorithm)(const CoreMessage * in));
	int SetTopicNodeIdSelectionAlgorithm(const CoreConfiguration * ctx, const Pattern * pattern, const Topic * topic, ServiceId * (*algorithm)(const CoreMessage * in));

	virtual ~FrontEndBase() {}

};

#endif // MQCloudCXX

