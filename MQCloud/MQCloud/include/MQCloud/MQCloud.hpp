// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches


//Note: all std::map can be replaced with folly or tbb analogs in future, current implementatiuon has only one dependency on protocol buffers. This topic shall be researched!

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
#include <mutex>
#include "CXX/GenericFunc.hpp"

namespace MQCloud {

// Delegates (lambda functions constructable/swig %template definable for OnAction overloading)
typedef GenericAction<> Task;
typedef GenericAction<const Message &> OnMessageAction;
typedef GenericAction<const UserMessage &> OnUserMessageAction;
typedef GenericAction<std::vector<UserMessage>> OnUserMessagesAction;
typedef GenericAction<std::vector<const std::string &>> OnNodesAction; // Gets a vector of ServiceId's
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
	@return connection Publishing socket
	*/
	virtual std::shared_ptr<Socket> CoreCreatePublishingSocket();
	
	/*
	ment to be sync
	@param connection socket which shall send message
	@param message
	*/
	virtual void CorePublishMessage(std::shared_ptr<Socket> socket, const Message & msg);

	virtual ~CorePublishingSocketBase();
};

// Subscribe socket (one to many)
struct CoreSubscriberSocketBase {
	/*
	ment to be sync
	@return connection Subscriber socket
	*/
	virtual std::shared_ptr<Socket> CoreCreateSubscribingSocket();

	/*
	@return connection socket
	@param connection Subscriber socket of this node
	@param callback on accepting new message 
	*/
	virtual std::shared_ptr<Socket> CoreConnectSubscribingSocket(std::shared_ptr<Socket> socket, const std::string & addr, std::shared_ptr<OnMessageAction> action);

	virtual ~CoreSubscriberSocketBase();
};


struct TaskId {
	virtual ~TaskId();
};

// Threading (for callbacks execution and core logic)
struct CoreThreadManagementBase {
	/*
	@param task to execute
	*/
	virtual void AddTask(std::shared_ptr<Task> action);


	/*
	@return task
	@param repeat each time
	@param task to execute
	*/
	virtual std::shared_ptr<TaskId> RepeatTask(int delayMilliseconds, std::shared_ptr<Task> action);

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

struct BackEndFactoryBase {
	virtual std::shared_ptr<CoreConfigurationBase> CreateContext();

	virtual ~BackEndFactoryBase() {}
};

struct ExtensiabiletyEventsHandlerBase {
	virtual void OnNodeConnectedToOther(const std::string & node);
	virtual void OnNodeConnectedToThis(const std::string & node);
	virtual void OnMessage(const Message & in);
	virtual void OnAdvertisedTopic(const std::string & pattern, const std::string & topic, const std::string & node);
	virtual void OnNodeRejectedTopic(const std::string & pattern, const std::string & topic, const std::string & node);

	virtual ~ExtensiabiletyEventsHandlerBase() {}
};

namespace internal {
	

	// For user on response handling
	struct ResponseHandler : std::enable_shared_from_this<ResponseHandler>, OnUserMessageAction {
		std::map<int, std::shared_ptr<OnUserMessageAction> > handlers;
		std::mutex mutex;
		virtual void OnAction(const UserMessage & m) {
			auto id = m.GetMessageId();
			
			std::unique_lock<std::mutex> lockHandlers(mutex);
			lockHandlers.lock();
			auto it = handlers.find(id);
			if(it != handlers.end()) {
				auto action = it->second;
				handlers.erase(it);
				lockHandlers.unlock();
				action->OnAction(m);
			}
		}

		void AddHandler(const int & id, std::shared_ptr<OnUserMessageAction> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			handlers[id] = handler;
		}

		void RemoveHandler(const int & id) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			auto it = handlers.find(id);
			if(it != handlers.end()) {
				handlers.erase(it);
			}
		}
	};

	// For user on request handling
	struct StaticResponseHandler : std::enable_shared_from_this<StaticResponseHandler>, OnMessageAction {
		std::map<std::string, std::shared_ptr<OnMessageAction> > handlers;
		std::mutex mutex;
		virtual void OnAction(const Message & m) {
			auto id = m.PatternName + ">";
			id += m.topic;
		
			std::unique_lock<std::mutex> lockHandlers(mutex);
			lockHandlers.lock();
			auto it = handlers.find(id);
			if(it != handlers.end()) {
				auto action = it->second;
				lockHandlers.unlock();
				action->OnAction(m);
			}
		}

		void AddHandler(const std::string & pattern, const std::string & topic, std::shared_ptr<OnUserMessageAction> handler) {
			auto id = pattern + ">" + topic;
			
			std::lock_guard<std::mutex> lockHandlers(mutex);
			handlers[id] = handler;
		}

		void RemoveHandler(const std::string & pattern, const std::string & topic) {
			auto id = pattern + ">" + topic;

			std::lock_guard<std::mutex> lockHandlers(mutex);
			auto it = handlers.find(id);
			if(it != handlers.end()) {
				handlers.erase(it);
			}
		}
	};

	struct GeneralMessageHandler : std::enable_shared_from_this<GeneralMessageHandler>, OnMessageAction {
		std::vector<std::shared_ptr<OnMessageAction> > handlers;
		std::mutex mutex;
		virtual void OnAction(const Message & m) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			for(auto & h : handlers) {
				h->OnAction(m);
			}
		}

		void AddHandler(std::shared_ptr<OnMessageAction> handler) {
			std::lock_guard<std::mutex> lockHandlers(mutex);
			handlers.push_back(handler);
		}
	};

	struct ConnectionsHandler {
		std::shared_ptr<CoreConfigurationBase> ctx;


		ConnectionsHandler(std::shared_ptr<CoreConfigurationBase> ctx) : ctx(ctx), inHandler(new GeneralMessageHandler), serviceId("-1") {
			Out = ctx->PublishingSocketInterface.CoreCreatePublishingSocket();
			In = ctx->SubscriberSocketInterface.CoreCreateSubscribingSocket();
		}

		void RemoveNode(const std::string & addr) {
			std::lock_guard<std::mutex> lockHandlers(connectionMutex);
			auto it = connections.find(addr);
			if(it != connections.end()) {
				connections.erase(it);
			}
		}

		void SubscribeToNode(const std::string & addr) {
			auto socket = ctx->SubscriberSocketInterface.CoreConnectSubscribingSocket(In, addr, inHandler);
			std::lock_guard<std::mutex> lockHandlers(connectionMutex);
			connections[socket->SocketId] = socket;
		}

		void AddOnMesageHandler(std::shared_ptr<OnMessageAction> handler) {
			inHandler->AddHandler(handler);
		}

		void PublishMessage(Message & m) {
			m.serviceId = serviceId;
			ctx->PublishingSocketInterface.CorePublishMessage(Out, m);
		}

		void SetServiceId(const std::string & id) {
			serviceId = id;
		}

		void SetHeartBeatRate(const int & heartbeatratems) {
			HeartBeat = ctx->ThreadingInterface.RepeatTask(heartbeatratems, std::make_shared<Task>([&](){SendHeartBeat();}));
		}

		void SendHeartBeat() {
			auto event = NodesManager::Protocol::HeartBeat();
			
			Message result;
			result.data = event.SerializeAsString();
			
			result.PatternName = "_HB";
			result.topic = "_";
		
			PublishMessage(result);

		}
	private:
		std::shared_ptr<Socket> Out;
		std::shared_ptr<Socket> In;
		std::map<std::string, std::shared_ptr<Socket> > connections;
		std::mutex connectionMutex;
		std::string serviceId;
		std::shared_ptr<TaskId> HeartBeat;

		std::shared_ptr<GeneralMessageHandler> inHandler;
	};


	struct MessagesManager {
		std::shared_ptr<ConnectionsHandler> ctx;
		MessagesManager(std::shared_ptr<ConnectionsHandler> ctx) : ctx(ctx) {}
	};


	struct SystemEventsManager : MessagesManager{
		// _SE
		const std::string serverPatternEvent;
		
		// _SN
		const std::string serverTopicNodeEvent;

		std::string runningBackEndName;

		explicit SystemEventsManager(std::shared_ptr<ConnectionsHandler> ctx, const std::string & runningBackEndName)
			: MessagesManager(ctx), runningBackEndName(runningBackEndName) {
		}

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

			ctx->PublishMessage(result);
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
		
			ctx->PublishMessage(result);
		}
	};

	struct SystemOperationsManager : MessagesManager{
		const int operationsSocketId = 0;
		
		// _SO
		const std::string serverPatternOperation;
		
		// _SG
		const std::string serverTopicNodeOperation;


		std::string runningBackEndName;

		std::shared_ptr<ResponseHandler> responseHandler;

		explicit SystemOperationsManager(std::shared_ptr<ConnectionsHandler> ctx, const std::string & runningBackEndName)	: MessagesManager(ctx), runningBackEndName(runningBackEndName), responseHandler(std::make_shared<ResponseHandler>()) {
			auto handler = std::make_shared<StaticResponseHandler>();
			handler->AddHandler(serverPatternOperation, serverTopicNodeOperation, responseHandler);
			ctx->AddOnMesageHandler(handler);
		}

		ResponseHandler GetAllPublishingNodesCallbacks;
		ResponseHandler GetAllSubscribedNodesCallbacks;

		void SendConnectRequest(const std::string desiredServiceName) {
			NodesManager::Protocol::ConnectRequest operation;
			operation.set_nodename(desiredServiceName);
			
			Message result;
			result.PatternName = serverPatternOperation;
			result.topic = serverTopicNodeOperation;
			
			auto action = std::make_shared<OnUserMessageAction> ([=](const UserMessage & m) {
				NodesManager::Protocol::ConnectResponse operation2;
				operation2.ParseFromString(m.GetData());
				ctx->SetServiceId(operation2.realnodename());
				ctx->SetHeartBeatRate(operation2.heartbeatratems());
			});
			responseHandler->AddHandler(result.GetMessageId(), action);
			ctx->PublishMessage(result);
		}

		void PrepareMessage(Message& result) {
			result.PatternName = serverPatternOperation;
			result.topic = serverTopicNodeOperation;
		}

		void SendSystemRequestGetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> handler) {
			static auto id = 0;
			NodesManager::Protocol::GetAllPublishersRequest operation;
			operation.set_pattern(pattern);
			operation.set_topic(topic);
			operation.set_backendname(runningBackEndName);
			Message result;
			PrepareMessage(result);

			auto action = std::make_shared<OnUserMessageAction> ([=](const UserMessage & m) {
				NodesManager::Protocol::GetAllPublishersResponse operation;
				operation.ParseFromString(m.GetData());

				std::vector<const std::string &> vec(operation.nodes().begin(), operation.nodes().end());
				handler->OnAction(vec);
			});

			responseHandler->AddHandler(result.GetMessageId(), action);
			ctx->PublishMessage(result);
		}

		void SendSystemRequestGetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<std::string(const Message *)> handler) {
			SendSystemRequestGetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
		}

		void SendSystemRequestGetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> handler) {
			static auto id = 0;
			NodesManager::Protocol::GetAllSubscribersRequest operation;
			operation.set_pattern(pattern);
			operation.set_topic(topic);
			operation.set_backendname(runningBackEndName);
			Message result;
			PrepareMessage(result);

			auto action = std::make_shared<OnUserMessageAction> ([=](const UserMessage & m) {
				NodesManager::Protocol::GetAllSubscribersResponse operation;
				operation.ParseFromString(m.GetData());

				std::vector<const std::string &> vec(operation.nodes().begin(), operation.nodes().end());
				handler->OnAction(vec);
			});

			responseHandler->AddHandler(result.GetMessageId(), action);
			ctx->PublishMessage(result);
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
		auto addr = runningCtx->PublishingSocketInterface.CoreCreatePublishingSocket(id);
		systemEventsManager.AdvertizeTopicEvent(pattern, topic);

	}
	void AdvertiseTopic(const std::string & pattern, const std::string & topic, std::function<void (std::vector<std::string>)> OnMessage);
	void RejectTopic(const std::string & pattern, const std::string & topic);

	void Subscribe(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::shared_ptr<OnMessageAction> OnMessage);
	void Subscribe(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::function<void (std::vector<std::string>)> OnMessage);
	void Unsubscribe(const std::string & pattern, const std::string & topic);

	void PublishMessageToAnyNode(const std::string & pattern, const std::string & topic, const Message & out);
	void PublishMessageToAllNodes(const std::string & pattern, const std::string & topic, const Message & out);
	void PublishMessageToNode(const std::string & node, const Message & out);
	void PublishMessageToNodes(const std::string & nodes, int nodesCount, const Message & out);

	void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllSubscribedNodes(pattern, topic, OnResult);		
	}

	void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::function<void (std::vector<std::string>)> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));		
	}

	void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult) {
		systemOperationsManager.SendSystemRequestGetAllPublishingNodes(pattern, topic, OnResult);		
	}
	void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<void (std::vector<std::string>)> OnResult) {
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
		void Request(const UserMessage & out, std::shared_ptr<OnUserMessageAction> OnMessage);
		void RequestTarget(const UserMessage &out, const std::string & target, std::shared_ptr<OnUserMessageAction> OnMessage);
		
		void AdvertizeReplysOnTopic(const std::string & topic, std::shared_ptr<OnUserMessageAction> OnMessage);
		void CloseReplysOnTopic(const std::string & topic);

		// Publish-Subscribe *(nonblocking)
		void AdvertizePublishingOnTopic(const std::string & topic);
		void PublishMessage(const UserMessage & out);
		void ClosePublishingOnTopic(const std::string & topic);


		void Subscribe(const std::string & topic, std::shared_ptr<OnUserMessageAction> OnMessage);
		void UnSubscribe(const std::string & topic);

		// Survey-Respondent *(nonblocking, nonforcing)
		void RequestMany(int MaxRespondents, int timeout, const UserMessage & out, std::shared_ptr<OnUserMessagesAction> OnReplyFromMany);
	};

}
#endif // MQCloudCXX

