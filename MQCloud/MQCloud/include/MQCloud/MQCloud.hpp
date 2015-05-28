// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches


#include <MQCloud/CXX/CStringAdaptor.hpp>
#include <MQCloud/CXX/Message.hpp>
#include <MQCloud/CXX/UserMessage.hpp>
#include <MQCloud/CXX/NodeManagerProtocol.pb.h>

#ifndef MQCloudCXX
#define MQCloudCXX
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
#include "CXX/GenericFunc.hpp"

namespace MQCloud {

// Delegates (lambda functions constructable/swig %template definable for OnAction overloading)
typedef GenericAction<const Message &> OnMessageAction;
typedef GenericAction<std::vector<std::string>> OnNodesAction; // Gets a vector of ServiceId's
typedef GenericFunc<std::string, const Message &> OnMessageAlgorithmAction; // returns ServiceId to send to


//Each core can work as async and/or sync.
//default assumption is that it  works as sync

struct Socket {
	std::string SocketId;

	//Shall close socket
	virtual ~Socket(){}
};

// Publish socket (one to many)
struct CorePublishingSocketBase {
	/*
	ment to be sync
	@param socketId which will be passed on message publish
	@return addres to which other endpoints could potentially bind
	*/
	virtual std::shared_ptr<Socket> CoreBindPublishingSocket();
	
	/*
	@param socketId which shall send message
	@param message
	*/
	virtual void CorePublishMessage(std::shared_ptr<Socket> socket, const Message & msg);

	virtual ~CorePublishingSocketBase();
};

// Subscribe socket (one to many)
struct CoreSubscriberSocketBase {
	/*
	@param connect to endpoint/node/peer etc.
	@param callback on accepting new message
	*/
	virtual std::shared_ptr<Socket> CoreConnectSubscribingSocket(const std::string & addr, std::shared_ptr<OnMessageAction> action);

	virtual ~CoreSubscriberSocketBase();
};


// Threading (for callbacks execution and core logic)
struct CoreThreadManagementBase {
	/*
	@return max allowed number of CoreCreateThread
	*/
	virtual int CoreGetAllowedThreadPoolMaxSize();

	/*
	@return created thread id
	*/
	virtual int CoreCreateThread();

	/*
	@param thread to delete id
	*/
	virtual void CoreDeleteThread(int threadId);

	/*
	@param thread id
	@param task to execute
	@param task to execute on task complete
	*/
	virtual void AddTask(int threadId, std::function<void()> task, std::function<void()> callback);

	virtual ~CoreThreadManagementBase();
};

struct CoreConfigurationBase {
	CoreThreadManagementBase ThreadingInterface;
	CoreSubscriberSocketBase SubscriberSocketInterface;
	CorePublishingSocketBase PublishingSocketInterface;

	std::string BackEndName; // BackEndName must include version only if one can not connect this backEnd to its older version

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
	virtual std::shared_ptr<CoreConfigurationBase> CreateContext();

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

namespace internal {

	struct ResponseHandler {
		std::shared_ptr<CoreConfigurationBase> ctx;
		ResponseHandler() {
			ctx->SubscriberSocketInterface.CoreConnectSubscribingSocket("bla");

		}
	};

	struct MessagesManager {
		std::shared_ptr<CoreConfigurationBase> ctx;
		MessagesManager(std::shared_ptr<CoreConfigurationBase> ctx) : ctx(ctx) {}
	};


	struct SystemEventsManager : MessagesManager{
		const int eventsSocketId = 1;

		// _SE
		const std::string serverPatternEvent;
		
		// _SN
		const std::string serverTopicNodeEvent;

		std::string runningBackEndName;

		explicit SystemEventsManager(const std::shared_ptr<CoreConfigurationBase>& ctx, const std::string & runningBackEndName)
			: MessagesManager(ctx), runningBackEndName(runningBackEndName) {}

		void SendOnNodeUnavaliableEvent(const std::string & pattern, const std::string & topic, const std::string & node) {
			auto event = NodesManager::Protocol::OnNodeUnavaliable();
			event.set_node(node);
			event.set_pattern(pattern);
			event.set_topic(topic);
			event.set_backendname(runningBackEndName);
			
			Message result;
			result.data = event.SerializeAsString();
			
			result.PatternName = serverPatternEvent;
			result.topic = serverTopicNodeEvent;
			ctx->PublishingSocketInterface->CorePublishMessage(eventsSocketId, result);
		}

		void AdvertizeTopicEvent(const std::string & pattern, const std::string & topic) {
			auto event = NodesManager::Protocol::AdvertizeTopic();
			event.set_pattern(pattern);
			event.set_topic(topic);
			event.set_backendname(runningBackEndName);
			
			Message result;
			result.data = event.SerializeAsString();
			
			result.PatternName = serverPatternEvent;
			result.topic = serverTopicNodeEvent;
			result.serviceId = //???
			ctx->PublishingSocketInterface->CorePublishMessage(eventsSocketId, result);
		}
	};

	struct SystemOperationsManager : MessagesManager{
		const int operationsSocketId = 0;
		
		// _SO
		const std::string serverPatternOperation;
		
		// _SG
		const std::string serverTopicNodeOperation;


		std::string runningBackEndName;

		explicit SystemOperationsManager(const std::shared_ptr<CoreConfigurationBase>& ctx, const std::string & runningBackEndName)	: MessagesManager(ctx), runningBackEndName(runningBackEndName) {}

		std::map<int, std::shared_ptr<OnNodesAction>> GetAllPublishingNodesCallbacks;
		std::map<int, std::shared_ptr<OnNodesAction>> GetAllSubscribedNodesCallbacks;

		void SendConnectRequest() {
			
		}

		void SendSystemRequestGetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> handler) {
			static auto id = 0;
			NodesManager::Protocol::GetAllPublishersRequest operation;
			operation.set_pattern(pattern);
			operation.set_topic(topic);
			operation.set_backendname(ctx->BackEndName);
			Message result;
			result.SetMessageId(id++);
			result.PatternName = serverPatternOperation;
			result.topic = serverTopicNodeOperation;

			GetAllPublishingNodesCallbacks[id] = handler;
			ctx->PublishingSocketInterface.CorePublishMessage(operationsSocketId, result);
		}

		void SendSystemRequestGetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<std::string(const Message *)> handler) {
			SendSystemRequestGetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
		}

		void SendSystemRequestGetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> handler) {
			static auto id = 0;
			NodesManager::Protocol::GetAllSubscribersRequest operation;
			operation.set_pattern(pattern);
			operation.set_topic(topic);
			operation.set_backendname(ctx->BackEndName);
			Message result;
			result.SetMessageId(id++);
			result.PatternName = serverPatternOperation;
			result.topic = serverTopicNodeOperation;

			GetAllPublishingNodesCallbacks[id] = handler;
			ctx->PublishingSocketInterface.CorePublishMessage(operationsSocketId, result);
		}

		void SendSystemRequestGetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::function<std::string(const Message *)> handler) {
			SendSystemRequestGetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
		}
	};
}

// Extensiabilety //
// operations will de treated as sync thus after CorePublishMessage call CoreMessage message deallocation will begin
// calls will be performed from one thread per socket
struct FrontEnd {
private:
	std::shared_ptr<CoreConfigurationBase> systemCtx;
	std::shared_ptr<CoreConfigurationBase> runningCtx;

	internal::SystemEventsManager systemEventsManager;
	internal::SystemOperationsManager systemOperationsManager;
	
	std::unordered_map<int, std::shared_ptr<ExtensiabiletyEventsHandlerBase>> handlers;
	int lastHandlerId;

	int lastPublisherId;
	std::unordered_map<std::string, int> Publishers;


public:
	FrontEnd(std::shared_ptr<CoreConfigurationBase> systemCtx, std::shared_ptr<CoreConfigurationBase> runningCtx) : systemCtx(systemCtx), 
		runningCtx(runningCtx),
		systemEventsManager(systemCtx, runningCtx->BackEndName),
		systemOperationsManager(systemCtx, runningCtx->BackEndName)), 
		lastHandlerId(0) {
		
	}

	int AddExtensiabiletyEventsHandler(std::shared_ptr<ExtensiabiletyEventsHandlerBase> handler) {
		handlers.insert(std::make_pair(lastHandlerId, handler));
		return lastHandlerId++;
	}

	void RemoveExtensiabiletyEventsHandler(int handlerId) {
		handlers.erase(handlerId);
	}

	//todo CLOSED BY
	void AdvertiseTopic(const std::string & pattern, const std::string & topic, std::shared_ptr<OnMessageAction> OnMessage) {
		auto id = lastPublisherId++;
		auto addr = runningCtx->PublishingSocketInterface.CoreBindPublishingSocket(id);
		systemEventsManager.AdvertizeTopicEvent(pattern, topic);

	}
	void AdvertiseTopic(const std::string & pattern, const std::string & topic, std::function<void (const ServiceId * nodes, int count)> OnMessage);
	void RejectTopic(const std::string & pattern, const std::string & topic);

	void Subscribe(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::shared_ptr<OnMessageAction> OnMessage);
	void Subscribe(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::function<void (const ServiceId * nodes, int count)> OnMessage);
	void Unsubscribe(const std::string & pattern, const std::string & topic);

	void PublishMessageToAnyNode(const std::string & pattern, const std::string & topic, std::shared_ptr<CoreMessage> out);
	void PublishMessageToAllNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<CoreMessage> out);
	void PublishMessageToNode(const std::string & node, std::shared_ptr<CoreMessage> out);
	void PublishMessageToNodes(const std::string & nodes, int nodesCount, std::shared_ptr<CoreMessage> out);

	void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllSubscribedNodes(pattern, topic, OnResult);		
	}

	void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::function<void (const ServiceId * nodes, int count)> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));		
	}

	void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllPublishingNodes(pattern, topic, OnResult);		
	}
	void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<void (const ServiceId * nodes, int count)> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));
	}

	//Load balancing
	int SetGeneralNodeIdSelectionAlgorithm(std::shared_ptr<OnMessageAlgorithmAction> algorithm);
	int SetGeneralNodeIdSelectionAlgorithm(std::function<std::string(const Message &)> algorithm);
	int SetTopicNodeIdSelectionAlgorithm(const std::string & pattern, const std::string & topic, std::shared_ptr<OnMessageAlgorithmAction> algorithm);
	int SetTopicNodeIdSelectionAlgorithm(const std::string & pattern, const std::string & topic, std::function<std::string(const Message &)> algorithm);
	
	void SetNodeCorrupted(const std::string & pattern, const std::string & topic, const std::string & node) {
		systemEventsManager.SendOnNodeUnavaliableEvent(pattern, topic, node);
	}

	virtual ~FrontEnd() {}
	std::string GetName();
};

// Throws if can not connect to the Exchange
struct API {
private:
	std::shared_ptr<FrontEnd> frontEnd;
public:
		API(std::shared_ptr<BackEndFactoryBase> runningBackEnd, std::shared_ptr<CoreConfigurationBase> exchangeCtx, const std::string & desiredName) {
			frontEnd = std::make_shared<FrontEnd>(exchangeCtx, runningBackEnd->CreateContext());

		}
		API(std::shared_ptr<BackEnd> runningBackEnd, const std::string & exchengeAdress, const std::string & desiredName) {}

		std::shared_ptr<FrontEnd> GetFrontEnd();
		std::string GetName() {
			return frontEnd->GetName();
		}
		std::shared_ptr<CoreConfigurationBase> GetExchangeContext();

		// Request-Reply, Request-Reply continuos *(nonblocking)
		void Request(const UserMessage & out, std::function<void (const UserMessage &)> OnMessage);
		void RequestTarget(const UserMessage &out, const std::string & target, std::function<void (const UserMessage &)> OnMessage);
		
		void AdvertizeReplysOnTopic(const std::string & topic, std::function<void (const UserMessage &)> OnMessage);
		void CloseReplysOnTopic(const std::string & topic);

		// Publish-Subscribe *(nonblocking)
		void AdvertizePublishingOnTopic(const std::string & topic);
		void PublishMessage(const UserMessage & out);
		void ClosePublishingOnTopic(const std::string & topic);


		void Subscribe(const std::string & topic, std::function<void (const UserMessage &)> OnMessage);
		void UnSubscribe(const std::string & topic);

		// Survey-Respondent *(nonblocking, nonforcing)
		void RequestMany(int MaxRespondents, int timeout, const UserMessage & out, std::function<void (const std::vector<UserMessage> & replys)> OnReplyFromMany);
	};

}
#endif // MQCloudCXX

