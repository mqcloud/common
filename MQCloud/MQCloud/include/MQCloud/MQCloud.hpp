// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches


//Note: all std::map can be replaced with folly or tbb analogs in future, current implementatiuon has only one dependency on protocol buffers. This topic shall be researched!

#include <MQCloud/CXX/CStringAdaptor.hpp>
#include <MQCloud/CXX/Message.hpp>
#include <MQCloud/CXX/UserMessage.hpp>
#include <MQCloud/CXX/Protocol.pb.h>

#ifndef MQCloudCXX
#define MQCloudCXX
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "CXX/GenericFunc.hpp"
#include "CXX/Protocol.pb.h"

namespace MQCloud
{
	// Delegates (lambda functions constructable/swig %template definable for OnAction overloading)
	typedef GenericAction<> Task;
	typedef GenericAction<const Message &> OnMessageAction;
	typedef GenericAction<const UserMessage &> OnUserMessageAction;
	typedef GenericAction<std::vector<UserMessage>> OnUserMessagesAction;
	typedef GenericAction<std::vector<const std::string &>> OnNodesAction; // Gets a vector of ServiceId's
	typedef GenericAction<const std::string &> OnNodeAction; // Get a ServiceId
	typedef GenericAction<std::string> OnConnectedAction;
	typedef GenericAction<> OnDeclaredOffline;
	typedef GenericAction<const std::string &> OnError;

	typedef GenericFunc<std::string, const Message &> OnMessageAlgorithmAction; // returns ServiceId to send to


	//Each core can work as async and/or sync.
	//default assumption is that it  works as sync

	struct Socket {
		std::string SocketId;
		virtual void AddDisconnectHandler(std::shared_ptr<OnError> handler);

		//Shall close socket
		virtual ~Socket() {}
	};

	// Publish socket (one to many)
	struct CorePublishingSocket {
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

		virtual ~CorePublishingSocket();
	};

	// Subscribe socket (one to many)
	struct CoreSubscriberSocket {
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

		virtual ~CoreSubscriberSocket();
	};


	struct TaskId {
		virtual ~TaskId();
	};

	// Threading (for callbacks execution and core logic)
	struct CoreThreadManagement {
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

		virtual ~CoreThreadManagement();
	};

	struct CoreConfiguration {
		CoreThreadManagement ThreadingInterface;
		CoreSubscriberSocket SubscriberSocketInterface;
		CorePublishingSocket PublishingSocketInterface;

		std::string BackEndName; // BackEndName must include version only if one can not connect this backEnd to its older version

		virtual ~CoreConfiguration() {}
	};


	struct BackEndFactory {
		virtual std::shared_ptr<CoreConfiguration> CreateContext();

		virtual ~BackEndFactory() {}
	};

	//for end users
	struct EventsHandler {
		virtual void OnExchangeError(const std::string & error);
		virtual void OnConnected();
		virtual void OnDisconnected();

		virtual void OnNewSubscriber(const std::string & topic, const std::string & subscriber);

		virtual ~EventsHandler() {}
	};

	//For pattern developers
	struct ExtensiabiletyEventsHandler {

		virtual void OnMessage(const Message & in);

		virtual void OnNodeConnectedToThis(const std::string & addr);
		virtual void OnNodeConnectedToOther(const std::string & addr);

		virtual void OnNodeAdvertisedTopic(const std::string & pattern, const std::string & topic, const std::string & node);
		virtual void OnNodeRejectedTopic(const std::string & pattern, const std::string & topic, const std::string & node);

		virtual void OnNodeSubscribedToTopic(const std::string & pattern, const std::string & topic, const std::string & node);
		virtual void OnNodeUnsubscribedFromTopic(const std::string & pattern, const std::string & topic, const std::string & node);

		virtual ~ExtensiabiletyEventsHandler() {}
	};

	namespace Internal
	{
		//TODO: Node manager!
		/*
	struct OutgoingConnectionsManager {
		void AddNode(const std::string & topic, std::shared_ptr<Socket> connection) {
			std::weak_ptr<Socket> weekConnectionPtr = connection;
			auto handler = std::make_shared<OnError>([&, weekConnectionPtr]{
				if(auto weekConnectionSrc = weekConnectionPtr.lock()) {
					std::lock_guard<std::mutex> lock_map(topicsMutex);
					auto topicIterator = topics.find(topic);
					if(topicIterator != topics.end()) {
						auto & vecRef (topicIterator->second);
						auto connectionIterator = find(vecRef.begin(), vecRef.end(), weekConnectionSrc);
						if(connectionIterator  != vecRef.end()) {
							vecRef.erase(connectionIterator);
						}
					}
				}
			});

			std::lock_guard<std::mutex> lock_map(topicsMutex);
			connection->AddDisconnectHandler(handler);
			topics[topic].push_back(connection);
		}

		void RemoveTopic(const std::string & topic) {
			std::lock_guard<std::mutex> l(topicsMutex);
			topics.erase(topic);
		}
	private:
		std::mutex topicsMutex;
		std::map<std::string, std::vector<std::shared_ptr<Socket>>> topics;
	};
	*/

		// For user on response handling
		struct ResponseHandler : std::enable_shared_from_this<ResponseHandler>, OnUserMessageAction {
			std::map<int, std::shared_ptr<OnUserMessageAction>> handlers;
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
			std::map<std::string, std::shared_ptr<OnMessageAction>> handlers;
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

		template<typename Targ = void>
		struct GenericSignalHandler : std::enable_shared_from_this<GenericSignalHandler<Targ>>, GenericAction<Targ> {

			std::vector<std::shared_ptr<GenericAction<Targ>>> handlers;
			std::mutex mutex;

			virtual void OnAction(Targ m) override {
				std::lock_guard<std::mutex> lockHandlers(mutex);
				for(auto & h : handlers) {
					h->OnAction(m);
				}
			}

			void AddHandler(std::shared_ptr<Tbase> handler) {
				std::lock_guard<std::mutex> lockHandlers(mutex);
				handlers.push_back(handler);
			}
		};

		template<>
		struct GenericSignalHandler<void> : std::enable_shared_from_this<GenericSignalHandler<>>, GenericAction<> {

			std::vector<std::shared_ptr<GenericAction<>>> handlers;
			std::mutex mutex;

			virtual void OnAction() override {
				std::lock_guard<std::mutex> lockHandlers(mutex);
				for(auto & h : handlers) {
					h->OnAction();
				}
			}

			void AddHandler(std::shared_ptr<GenericAction<>> handler) {
				std::lock_guard<std::mutex> lockHandlers(mutex);
				handlers.push_back(handler);
			}
		};

		struct GeneralMessageHandler : GenericSignalHandler<const Message &> {};

		struct GeneralStringHandler : GenericSignalHandler<const std::string &> {};

		struct GeneralTaskHandler : GenericSignalHandler<void> {};

		struct ConnectionsHandler {
			ConnectionsHandler(std::shared_ptr<CoreConfiguration> ctx) : ctx(ctx), serviceId("-1"), inHandler(new GeneralMessageHandler) {
				OnDisconnected->AddHandler(std::make_shared<OnError>([&](const std::string & error) {
					                                                     std::lock_guard<std::mutex> lock_map(connectionMutex);
					                                                     subscribtionCnnections.erase(subscribtionCnnections.begin(), subscribtionCnnections.end());
				                                                     }));

				Out = ctx->PublishingSocketInterface.CoreCreatePublishingSocket();
				Out->AddDisconnectHandler(OnDisconnected);
				subscribingSocket = ctx->SubscriberSocketInterface.CoreCreateSubscribingSocket();
				subscribingSocket->AddDisconnectHandler(OnDisconnected);

			}

			void RemoveNode(const std::string & addr) {
				std::lock_guard<std::mutex> lockHandlers(connectionMutex);
				subscribtionCnnections.erase(addr);
			}

			void ConnectToExchangeNode(const std::string & addr, std::shared_ptr<OnError> onError) {
				auto connection = ctx->SubscriberSocketInterface.CoreConnectSubscribingSocket(subscribingSocket, addr, inHandler);
				auto id = connection->SocketId;
				std::weak_ptr<Socket> weekConnectionPtr = connection;
				OnExchangeError->AddHandler(onError);

				auto handler = std::make_shared<OnError>([&, weekConnectionPtr, id](const std::string & error) {
					                                         if(auto weekConnectionSrc = weekConnectionPtr.lock()) {
						                                         std::lock_guard<std::mutex> lock_map(connectionMutex);
						                                         subscribtionCnnections.erase(id);
					                                         }
					                                         OnExchangeError->OnAction(error);
				                                         });

				connection->AddDisconnectHandler(handler);

				std::lock_guard<std::mutex> lockHandlers(connectionMutex);
				subscribtionCnnections[id] = connection;
			}

			void ConnectToNode(const std::string & addr) {
				auto connection = ctx->SubscriberSocketInterface.CoreConnectSubscribingSocket(subscribingSocket, addr, inHandler);
				auto id = connection->SocketId;
				std::weak_ptr<Socket> weekConnectionPtr = connection;

				auto handler = std::make_shared<OnError>([&, weekConnectionPtr, id] {
					                                         if(auto weekConnectionSrc = weekConnectionPtr.lock()) {
						                                         std::lock_guard<std::mutex> lock_map(connectionMutex);
						                                         subscribtionCnnections.erase(id);
					                                         }
				                                         });

				connection->AddDisconnectHandler(handler);

				std::lock_guard<std::mutex> lockHandlers(connectionMutex);
				subscribtionCnnections[id] = connection;
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
				HeartBeatRate = heartbeatratems;
			}

			int GetHeartBeatRate() const {
				return HeartBeatRate;
			}

			std::shared_ptr<GeneralStringHandler> OnExchangeError;
			std::shared_ptr<GeneralStringHandler> OnDisconnected;
			std::shared_ptr<GeneralTaskHandler> OnNodeConnectedToOther;
			std::shared_ptr<GeneralStringHandler> OnNodeConnectedToThis;
		private:
			std::shared_ptr<CoreConfiguration> ctx;
			std::shared_ptr<Socket> Out;
			std::shared_ptr<Socket> subscribingSocket;
			std::map<std::string, std::shared_ptr<Socket>> subscribtionCnnections;
			std::mutex connectionMutex;
			std::string serviceId;
			int HeartBeatRate;
			std::shared_ptr<GeneralMessageHandler> inHandler;

			void OnExternalNodeSocketDisconnect(const std::string & error) {
				//TODO Clear handlers
			}

			void OnHostNodeSocketDisconnect(const std::string & error) {
				//TODO Exchange Error, close up
			}

		};


		struct MessagesManager {
			std::shared_ptr<ConnectionsHandler> ctx;

			MessagesManager(std::shared_ptr<ConnectionsHandler> ctx) : ctx(ctx) {}
		};


		struct StaticEventsHandler : std::enable_shared_from_this<StaticEventsHandler>, OnMessageAction {
			std::shared_ptr<GeneralMessageHandler> generalMessageHandler;

			StaticEventsHandler() {
				generalMessageHandler = std::make_shared<GeneralMessageHandler>();
			}

			virtual void OnAction(const Message & m) {
				Protocol::IncomingEvent event;
				event.ParseFromString(m.data);

				//TODO: fill the gaps
				switch(event.typecode()) {
					case Protocol::IncomingEventTypeOnConnectionClosed: break;
					case Protocol::IncomingEventTypeOnConnectionEstablished: break;
					case Protocol::IncomingEventTypeOnNodeAdvertisedTopic: break;
					case Protocol::IncomingEventTypeOnNodeRejectedTopic: break;
					case Protocol::IncomingEventTypeOnNodeSubscribedToTopic: break;
					case Protocol::IncomingEventTypeOnNodeUnavaliable: break;
					case Protocol::IncomingEventTypeOnNodeUnsubscribedFromTopic: break;
					default: break;


				}


			}
		};


		struct SystemEventsManager : MessagesManager {
			// _SE
			const std::string serverPatternEvent;

			// _SN
			const std::string serverTopicNodeEvent;
			const std::string serverTopicHeartBeat;

			const std::string runningBackEndName;

			std::shared_ptr<ResponseHandler> eventsHandler;


			explicit SystemEventsManager(std::shared_ptr<ConnectionsHandler> ctx, const std::string & runningBackEndName)
				: MessagesManager(ctx), serverPatternEvent("_E"), serverTopicNodeEvent("_SE"), serverTopicHeartBeat("_SHB"), runningBackEndName(runningBackEndName) {
				auto handler = std::make_shared<StaticResponseHandler>();
				handler->AddHandler(serverPatternEvent, serverTopicNodeEvent, eventsHandler);
				ctx->AddOnMesageHandler(handler);
			}

			void UnsubscribeEvent(const std::string & pattern, const std::string & topic) {
				auto event = Protocol::UnSubscribe();
				event.set_pattern(pattern);
				event.set_topic(topic);

				Protocol::OutgoingEvent outgoingMessage;

				outgoingMessage.set_typecode(Protocol::OutgoingEventTypeUnSubscribe);
				outgoingMessage.set_allocated_unsubscribe(&event);

				auto serialized = outgoingMessage.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				ctx->PublishMessage(result);
			}

			void SubscribeEvent(const std::string & pattern, const std::string & topic) {
				auto event = Protocol::Subscribe();
				event.set_pattern(pattern);
				event.set_topic(topic);

				Protocol::OutgoingEvent outgoingMessage;

				outgoingMessage.set_typecode(Protocol::OutgoingEventTypeSubscribe);
				outgoingMessage.set_allocated_subscribe(&event);

				auto serialized = outgoingMessage.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				ctx->PublishMessage(result);
			}

			void ReportNodeUnavaliable(const std::string & pattern, const std::string & topic, const std::string & node) {
				auto event = Protocol::ReportNodeUnavaliable();
				event.set_node(node);
				event.set_pattern(pattern);
				event.set_topic(topic);

				Protocol::OutgoingEvent outgoingMessage;

				outgoingMessage.set_typecode(Protocol::OutgoingEventTypeReportNodeUnavaliable);
				outgoingMessage.set_allocated_reportnodeunavaliable(&event);

				auto serialized = outgoingMessage.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				ctx->PublishMessage(result);
			}

			void RejectTopicEvent(const std::string & pattern, const std::string & topic) {
				auto event = Protocol::RejectTopic();
				event.set_pattern(pattern);
				event.set_topic(topic);

				Protocol::OutgoingEvent outgoingMessage;

				outgoingMessage.set_typecode(Protocol::OutgoingEventTypeRejectTopic);
				outgoingMessage.set_allocated_rejecttopic(&event);

				auto serialized = outgoingMessage.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				ctx->PublishMessage(result);
			}

			void AdvertizeTopicEvent(const std::string & pattern, const std::string & topic) {
				auto event = Protocol::AdvertizeTopic();
				event.set_pattern(pattern);
				event.set_topic(topic);

				Protocol::OutgoingEvent outgoingMessage;

				outgoingMessage.set_typecode(Protocol::OutgoingEventTypeAdvertizeTopic);
				outgoingMessage.set_allocated_advertizetopic(&event);

				auto serialized = outgoingMessage.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				ctx->PublishMessage(result);
			}

			void DisconnectEvent() {
				auto event = Protocol::Disconnect();

				Protocol::OutgoingEvent outgoingMessage;

				outgoingMessage.set_typecode(Protocol::OutgoingEventTypeDisconnect);
				outgoingMessage.set_allocated_disconnect(&event);

				auto serialized = outgoingMessage.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				ctx->PublishMessage(result);
			}

			void HeartBeatEvent() {
				auto event = Protocol::HeartBeat();

				Message result;
				result.data = event.SerializeAsString();

				result.PatternName = serverPatternEvent;
				result.topic = serverTopicHeartBeat;

				ctx->PublishMessage(result);
			}

		private:
			void PrepareMessage(Message & result) {
				result.PatternName = serverPatternEvent;
				result.topic = serverTopicNodeEvent;
			}
		};

		struct SystemOperationsManager : MessagesManager {
			const int operationsSocketId = 0;

			// _SO
			const std::string serverPatternOperation;

			// _SG
			const std::string serverTopicNodeOperation;


			std::string runningBackEndName;

			std::shared_ptr<ResponseHandler> responseHandler;

			explicit SystemOperationsManager(std::shared_ptr<ConnectionsHandler> ctx, const std::string & runningBackEndName) : MessagesManager(ctx), serverPatternOperation("_SPO"), serverTopicNodeOperation("_SO"),
			                                                                                                                    runningBackEndName(runningBackEndName), responseHandler(std::make_shared<ResponseHandler>()) {
				auto handler = std::make_shared<StaticResponseHandler>();
				handler->AddHandler(serverPatternOperation, serverTopicNodeOperation, responseHandler);
				ctx->AddOnMesageHandler(handler);
			}

			void ConnectRequest(const std::string & desiredServiceName, std::shared_ptr<OnConnectedAction> handler) {
				Protocol::ConnectRequest request;
				request.set_nodename(desiredServiceName);
				request.set_backendname(runningBackEndName);

				Protocol::OutgoingOperation operation;

				operation.set_typecode(Protocol::OutgoingOperationTypeConnectRequest);
				operation.set_allocated_connectrequest(&request);

				auto serialized = operation.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage & m) {
					                                                    Protocol::IncomingOperation incomingOperation;
					                                                    incomingOperation.ParseFromString(m.GetData());

					                                                    const auto & response(incomingOperation.connectresponse());

					                                                    ctx->SetServiceId(response.realnodename());
					                                                    ctx->SetHeartBeatRate(response.heartbeatratems());

					                                                    handler->OnAction(response.realnodename());
				                                                    });
				responseHandler->AddHandler(result.GetMessageId(), action);
				ctx->PublishMessage(result);
			}

			void ConnectRequest(const std::string & desiredServiceName, std::function<std::string> handler) {
				ConnectRequest(desiredServiceName, std::make_shared<OnConnectedAction>(handler));
			}

			void PrepareMessage(Message & result) {
				result.PatternName = serverPatternOperation;
				result.topic = serverTopicNodeOperation;
			}

			void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> handler) {
				Protocol::GetAllPublishersRequest request;
				request.set_pattern(pattern);
				request.set_topic(topic);

				Protocol::OutgoingOperation operation;

				operation.set_typecode(Protocol::OutgoingOperationTypeGetAllPublishersRequest);
				operation.set_allocated_getallpublishersrequest(&request);

				auto serialized = operation.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage & m) {
					                                                    Protocol::IncomingOperation incomingOperation;
					                                                    incomingOperation.ParseFromString(m.GetData());

					                                                    const auto & response(incomingOperation.getallpublishersresponse());

					                                                    std::vector<const std::string &> vec(response.nodes().begin(), response.nodes().end());
					                                                    handler->OnAction(vec);
				                                                    });

				responseHandler->AddHandler(result.GetMessageId(), action);
				ctx->PublishMessage(result);
			}

			void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<std::vector<const std::string &>> handler) {
				GetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
			}

			void GetPublisherNode(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodeAction> handler) {
				Protocol::GetPublisherRequest request;
				request.set_pattern(pattern);
				request.set_topic(topic);

				Protocol::OutgoingOperation operation;

				operation.set_typecode(Protocol::OutgoingOperationTypeGetAllSubscribersRequest);
				operation.set_allocated_getpublisherrequest(&request);

				auto serialized = operation.SerializeAsString();

				Message result;
				result.data = serialized;

				PrepareMessage(result);

				auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage & m) {
					                                                    Protocol::IncomingOperation incomingOperation;
					                                                    incomingOperation.ParseFromString(m.GetData());

					                                                    const auto & response(incomingOperation.getpublisherresponse());

					                                                    handler->OnAction(response.node());
				                                                    });

				responseHandler->AddHandler(result.GetMessageId(), action);
				ctx->PublishMessage(result);
			}

			void GetPublisherNode(const std::string & pattern, const std::string & topic, std::function<const std::string &> handler) {
				GetPublisherNode(pattern, topic, std::make_shared<OnNodeAction>(move(handler)));
			}

			void GetSubscriberNode(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodeAction> handler) {
				Protocol::GetSubscriberRequest request;
				request.set_pattern(pattern);
				request.set_topic(topic);

				Protocol::OutgoingOperation operation;

				operation.set_typecode(Protocol::OutgoingOperationTypeGetSubscriberRequest);
				operation.set_allocated_getsubscriberrequest(&request);

				auto serialized = operation.SerializeAsString();
				Message result;
				result.data = serialized;
				PrepareMessage(result);

				auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage & m) {
					                                                    Protocol::IncomingOperation incomingOperation;
					                                                    incomingOperation.ParseFromString(m.GetData());

					                                                    const auto & response(incomingOperation.getsubscriberresponse());
					                                                    handler->OnAction(response.node());
				                                                    });

				responseHandler->AddHandler(result.GetMessageId(), action);
				ctx->PublishMessage(result);
			}

			void GetSubscriberNode(const std::string & pattern, const std::string & topic, std::function<const std::string &> handler) {
				GetSubscriberNode(pattern, topic, std::make_shared<OnNodeAction>(move(handler)));
			}

			void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> handler) {
				Protocol::GetAllSubscribersRequest request;
				request.set_pattern(pattern);
				request.set_topic(topic);

				Protocol::OutgoingOperation operation;

				operation.set_typecode(Protocol::OutgoingOperationTypeGetAllSubscribersRequest);
				operation.set_allocated_getallsubscribersrequest(&request);

				auto serialized = operation.SerializeAsString();

				Message result;
				result.data = serialized;
				PrepareMessage(result);

				auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage & m) {
					                                                    Protocol::IncomingOperation incomingOperation;
					                                                    incomingOperation.ParseFromString(m.GetData());

					                                                    const auto & response(incomingOperation.getallsubscribersresponse());

					                                                    std::vector<const std::string &> vec(response.nodes().begin(), response.nodes().end());
					                                                    handler->OnAction(vec);
				                                                    });

				responseHandler->AddHandler(result.GetMessageId(), action);
				ctx->PublishMessage(result);
			}

			void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::function<std::vector<const std::string &>> handler) {
				GetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
			}

		};
	}

	// Extensiabilety //
	// operations will de treated as sync thus after CorePublishMessage call CoreMessage message deallocation will begin
	// calls will be performed from one thread per socket
	struct FrontEnd {
	private:
		std::shared_ptr<CoreConfiguration> systemCtx;
		std::shared_ptr<CoreConfiguration> runningCtx;

		Internal::SystemEventsManager systemEventsManager;
		Internal::SystemOperationsManager systemOperationsManager;
		std::shared_ptr<Internal::ConnectionsHandler> systemConnectionHandler;
		std::shared_ptr<Internal::ConnectionsHandler> runningConnectionHandler;
		std::unordered_map<int, std::shared_ptr<ExtensiabiletyEventsHandler>> handlers;
		int lastHandlerId;

		int lastPublisherId;
		std::unordered_map<std::string, int> Publishers;

		std::shared_ptr<Task> HeartBeat;
	public:
		FrontEnd(std::shared_ptr<CoreConfiguration> systemCtx, std::shared_ptr<CoreConfiguration> runningCtx) : systemCtx(systemCtx),
		                                                                                                        runningCtx(runningCtx),
		                                                                                                        systemConnectionHandler(std::make_shared<internal::ConnectionsHandler>(systemCtx)),
		                                                                                                        systemEventsManager(systemConnectionHandler, runningCtx->BackEndName),
		                                                                                                        systemOperationsManager(systemConnectionHandler, runningCtx->BackEndName),
		                                                                                                        lastHandlerId(0) { }

		FrontEnd(std::shared_ptr<CoreConfiguration> runningCtx, const std::string & systemAdr) : systemCtx(systemCtx),
		                                                                                         runningCtx(runningCtx),
		                                                                                         systemConnectionHandler(std::make_shared<internal::ConnectionsHandler>(runningCtx)),
		                                                                                         systemEventsManager(systemConnectionHandler, runningCtx->BackEndName),
		                                                                                         systemOperationsManager(systemConnectionHandler, runningCtx->BackEndName),
		                                                                                         runningConnectionHandler(systemConnectionHandler),
		                                                                                         lastHandlerId(0) { }

		void Connect(const std::string & serviceName, std::shared_ptr<OnConnectedAction> OnConnected) {
			auto handler = std::make_shared<OnConnectedAction>([=](const std::string & id) {
				                                                   HeartBeat = systemCtx->ThreadingInterface.RepeatTask(systemConnectionHandler->GetHeartBeatRate(), std::make_shared<Task>([&]() {
					                                                                                                                                                                            SendHeartBeat();
				                                                                                                                                                                            }));
				                                                   OnConnected->OnAction(id);
			                                                   });
			systemOperationsManager.ConnectRequest(serviceName, handler);
		}

		int AddExtensiabiletyEventsHandler(std::shared_ptr<ExtensiabiletyEventsHandler> handler) {
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
			systemOperationsManager.GetAllSubscribedNodes(pattern, topic, OnResult);
		}

		void GetAllSubscribedNodes(const std::string & pattern, const std::string & topic, std::function<void (std::vector<std::string>)> OnResult) {
			systemOperationsManager.GetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));
		}

		void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::shared_ptr<OnNodesAction> OnResult) {
			systemOperationsManager.GetAllPublishingNodes(pattern, topic, OnResult);
		}

		void GetAllPublishingNodes(const std::string & pattern, const std::string & topic, std::function<void (std::vector<std::string>)> OnResult) {
			systemOperationsManager.GetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));
		}

		void GetAllConnectedNodes(std::shared_ptr<OnNodesAction> OnResult);
		void GetAllConnectedNodes(std::function<void (std::vector<std::string>)> OnResult);

		//Load balancing
		int SetGeneralNodeIdSelectionAlgorithm(std::shared_ptr<OnMessageAlgorithmAction> algorithm);
		int SetGeneralNodeIdSelectionAlgorithm(std::function<std::string(const Message &)> algorithm);
		int SetTopicNodeIdSelectionAlgorithm(const std::string & pattern, const std::string & topic, std::shared_ptr<OnMessageAlgorithmAction> algorithm);
		int SetTopicNodeIdSelectionAlgorithm(const std::string & pattern, const std::string & topic, std::function<std::string(const Message &)> algorithm);

		void SetNodeCorrupted(const std::string & pattern, const std::string & topic, const std::string & node) {
			systemEventsManager.renavaliableEvent(pattern, topic, node);
		}

		virtual ~FrontEnd() {}

		std::string GetName();
	};

	// Throws if can not connect to the Exchange
	struct API {
	private:
		std::shared_ptr<FrontEnd> frontEnd;
	public:
		API(std::shared_ptr<BackEndFactory> runningBackEnd, std::shared_ptr<CoreConfiguration> exchangeCtx) {
			frontEnd = std::make_shared<FrontEnd>(exchangeCtx, runningBackEnd->CreateContext());
		}

		API(std::shared_ptr<BackEndFactory> runningBackEnd, const std::string & exchengeAdress) {
			frontEnd = std::make_shared<FrontEnd>(runningBackEnd, exchengeAdress);
		}


		void Connect(const std::string & desiredName, std::shared_ptr<OnConnectedAction> handler) {
			frontEnd->Connect(desiredName, handler);
		}

		std::shared_ptr<FrontEnd> GetFrontEnd();

		std::string GetName() {
			return frontEnd->GetName();
		}

		std::shared_ptr<CoreConfiguration> GetExchangeContext();

		// Request-Reply, Request-Reply continuos *(nonblocking)
		void Request(const UserMessage & out, std::shared_ptr<OnUserMessageAction> OnMessage);
		void RequestTarget(const UserMessage & out, const std::string & target, std::shared_ptr<OnUserMessageAction> OnMessage);

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

