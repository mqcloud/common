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
#include <utility>

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


struct OnMessageAction {
	explicit OnMessageAction(const OnMessageAction & other)
		: _action(other._action) {}

	explicit OnMessageAction(const std::function<void(const Message *)> & action) : _action(action) {}

	OnMessageAction(OnMessageAction && other)
		: _action(std::move(other._action)) {}

	OnMessageAction() {}

	OnMessageAction(std::function<void(const Message *)> && action) : _action(action) {}

	virtual void OnMessage(const Message * msg) {
		if(_action) {
			_action(msg);
		}
	}

	virtual ~OnMessageAction() {}

private:
	std::function<void(const Message *)> _action;
};

struct OnNodesAction {
	explicit OnNodesAction(const OnNodesAction & other)
		: _action(other._action) {}

	explicit OnNodesAction(const std::function<void (const ServiceId * nodes, int count)> & action) : _action(action) {}


	OnNodesAction(OnNodesAction && other)
		: _action(std::move(other._action)) {}

	OnNodesAction() {}

	OnNodesAction(std::function<void(const ServiceId * nodes, int count)> && action) : _action(action) {}

	virtual void OnMessage(const ServiceId * nodes, int count) {
		if(_action) {
			_action(nodes, count);
		}
	}

	virtual ~OnNodesAction() {}

private:
	std::function<void (const ServiceId * nodes, int count)> _action;
};

struct OnMessageAlgorithmAction {
	explicit OnMessageAlgorithmAction(const OnMessageAlgorithmAction & other)
		: _action(other._action) {}

	explicit OnMessageAlgorithmAction(const std::function<std::string(const Message *)> & action) : _action(action) {}

	OnMessageAlgorithmAction(OnMessageAlgorithmAction && other)
		: _action(std::move(other._action)) {}

	OnMessageAlgorithmAction() {}

	OnMessageAlgorithmAction(std::function<std::string(const Message *)> && action) : _action(action) {}

	virtual void OnMessage(const Message * msg) {
		if(_action) {
			_action(msg);
		}
	}

	virtual ~OnMessageAlgorithmAction() {}

private:
	std::function<std::string(const Message *)> _action;
};

struct FrontEndBase {
	FrontEndBase(std::shared_ptr<CoreConfiguration> ctx);
	virtual int AddExtensiabiletyEventsHandler(std::shared_ptr<ExtensiabiletyEventsHandler> handler);
	virtual void RemoveExtensiabiletyEventsHandler(int handlerId);

	virtual void AdvertiseTopic(const std::string & pattern, const std::string & topic, std::shared_ptr<OnMessageAction> OnMessage);
	virtual void AdvertiseTopic(const std::string & pattern, const std::string & topic, std::function<void (const ServiceId * nodes, int count)> OnMessage);
	virtual void RejectTopic(const std::string & pattern, const std::string & topic);

	virtual void Subscribe(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::shared_ptr<OnMessageAction> OnMessage);
	virtual void Subscribe(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::function<void (const ServiceId * nodes, int count)> OnMessage);
	virtual void Unsubscribe(const std::string & pattern, const std::string & topic);

	virtual void PublishMessageToAnyNode(const std::string & pattern, const std::string & topic, std::shared_ptr<CoreMessage> out);
	virtual void PublishMessageToAllNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<CoreMessage> out);
	virtual void PublishMessageToNode(const std::string & node, std::shared_ptr<CoreMessage> out);
	virtual void PublishMessageToNodes(const std::string & nodes, int nodesCount, std::shared_ptr<CoreMessage> out);

	virtual void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult);
	virtual void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::function<void (const ServiceId * nodes, int count)> OnResult);
	virtual void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult);
	virtual void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<void (const ServiceId * nodes, int count)> OnResult);

	//Load balancing
	int SetGeneralNodeIdSelectionAlgorithm(std::shared_ptr<OnMessageAlgorithmAction> algorithm);
	int SetGeneralNodeIdSelectionAlgorithm(std::function<std::string(const Message *)> algorithm);
	int SetTopicNodeIdSelectionAlgorithm(const std::string & pattern, const std::string & topic, std::shared_ptr<OnMessageAlgorithmAction> algorithm);
	int SetTopicNodeIdSelectionAlgorithm(const std::string & pattern, const std::string & topic, std::function<std::string(const Message *)> algorithm);

	virtual ~FrontEndBase() {}
};

#endif // MQCloudCXX

