#ifndef FRONTEND_HPP
#define  FRONTEND_HPP
#include <MQCloud/CXX/Internal/ExchangeOperationsManager.hpp>
#include <MQCloud/CXX/Internal/ExchangeEventsManager.hpp>
#include <MQCloud/CXX/PublishingSocket.hpp>
#include <MQCloud/CXX/ExtensiabiletyEventsHandler.hpp>

#include <unordered_map>


namespace MQCloud {
	// Extensiabilety //
	// operations will de treated as sync thus after CorePublishMessage call CoreMessage message deallocation will begin
	// calls will be performed from one thread per socket
	struct FrontEnd :
#ifndef SWIG
		std::enable_shared_from_this<FrontEnd>
#endif
	{
	private:
		std::shared_ptr<BackEndConfiguration> exchangeCtx;
		std::shared_ptr<BackEndConfiguration> runningCtx;

		Internal::ExchangeEventsManager systemEventsManager;
		Internal::ExchangeOperationsManager systemOperationsManager;
		std::shared_ptr<Internal::ConnectionsHandler> systemConnectionHandler;
		std::shared_ptr<Internal::ConnectionsHandler> runningConnectionHandler;
		std::unordered_map<int, std::shared_ptr<ExtensiabiletyEventsHandler>> handlers;
		int lastHandlerId;

		int lastPublisherId;
		std::unordered_map<std::string, int> Publishers;

		std::shared_ptr<Task> HeartBeat;

		std::shared_ptr<ThreadManagementInterface> ThreadingInterface;
	public:
		FrontEnd(std::shared_ptr<BackEndConfiguration> systemCtx, std::shared_ptr<BackEndConfiguration> runningCtx) :
			exchangeCtx(systemCtx),
			runningCtx(runningCtx),
			systemConnectionHandler(std::make_shared<Internal::ConnectionsHandler>(systemCtx)),
			systemEventsManager(systemConnectionHandler, runningCtx->BackEndName),
			systemOperationsManager(systemConnectionHandler, runningCtx->BackEndName),
			lastHandlerId(0),
			ThreadingInterface(std::make_shared<ThreadManager>())
			{ }

		FrontEnd(std::shared_ptr<BackEndConfiguration> runningCtx, const std::string& systemAdr) :
			exchangeCtx(exchangeCtx),
			runningCtx(runningCtx),
			systemConnectionHandler(std::make_shared<Internal::ConnectionsHandler>(runningCtx)),
			systemEventsManager(systemConnectionHandler, runningCtx->BackEndName),
			systemOperationsManager(systemConnectionHandler, runningCtx->BackEndName),
			runningConnectionHandler(systemConnectionHandler),
			lastHandlerId(0),
			ThreadingInterface(std::make_shared<ThreadManager>())
		{ }

		void Connect(const std::string& serviceName, std::shared_ptr<OnConnectedAction> OnConnected) {
			auto instance = shared_from_this();
			auto handler = std::make_shared<OnConnectedAction>([=](const std::string& id) {
				instance->runningConnectionHandler->SetServiceId(serviceName);
				instance->systemConnectionHandler->SetServiceId(serviceName);
				instance->HeartBeat = ThreadingInterface->RepeatTask(systemConnectionHandler->GetHeartBeatRate(),
					std::make_shared<Task>([&]() {
						systemEventsManager.HeartBeatEvent();
						OnConnected->OnAction(id);
				}));

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

		void AdvertiseTopic(const std::string& pattern, const std::string& topic) {
			systemEventsManager.AdvertizeTopicEvent(pattern, topic);
		}

		void AdvertiseTopic(const std::string& pattern, const std::string& topic, std::shared_ptr<OnNodesAction> CurrentSubscribers) {
			systemEventsManager.exchangeEventsHandler->OnNodeAdvertisedTopic([](const std::string &, const std::string &, const std::string &) {
				
			});
			systemEventsManager.AdvertizeTopicEvent(pattern, topic);
		}

		void RejectTopic(const std::string& pattern, const std::string& topic);

		void Subscribe(const std::string& pattern, const std::string& topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::shared_ptr<OnMessageAction> OnMessage);
		void Subscribe(const std::string& pattern, const std::string& topic, std::shared_ptr<OnNodesAction> OnSubscribed, std::function<void (const Message&)> OnMessage);

		void RegisterResponsesHandler(const std::string& pattern, const std::string& topic);
		void RegisterResponseCalback(const std::string& pattern, const std::string& topic, int messageId, std::shared_ptr<OnMessageAction> OnMessage);
		void RegisterResponseCalback(const std::string& pattern, const std::string& topic, int messageId, std::function<void (const Message&)> OnMessage);
		void RegisterMultipleResponsesCalback(const std::string& pattern, const std::string& topic, int message_id, int timeoutDeadlineMilliseconds, std::shared_ptr<OnMessagesAction> OnMessage);
		void RegisterMultipleResponsesCalback(const std::string& pattern, const std::string& topic, int message_id, int timeoutDeadlineMilliseconds, std::function<void (std::vector<Message>)> OnMessage);

		void Unsubscribe(const std::string& pattern, const std::string& topic);

		void PublishMessageToAnyNode(const Message& out);
		void PublishMessageToAllNodes(const Message& out);
		void PublishMessageToNode(const std::string& node, const Message& out);
		void PublishMessageToNodes(const std::vector<std::string>& nodes, const Message& out);


		void GetAllSubscribedNodes(const std::string& pattern, const std::string& topic, std::shared_ptr<OnNodesAction> OnResult) {
			systemOperationsManager.GetAllSubscribedNodes(pattern, topic, OnResult);
		}

		void GetAllSubscribedNodes(const std::string& pattern, const std::string& topic, std::function<void (const std::vector<std::string>&)> OnResult) {
			systemOperationsManager.GetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));
		}

		void GetAllPublishingNodes(const std::string& pattern, const std::string& topic, std::shared_ptr<OnNodesAction> OnResult) {
			systemOperationsManager.GetAllPublishingNodes(pattern, topic, OnResult);
		}

		void GetAllPublishingNodes(const std::string& pattern, const std::string& topic, std::function<void (std::vector<std::string>)> OnResult) {
			systemOperationsManager.GetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(OnResult));
		}

		void GetAllConnectedNodes(std::shared_ptr<OnNodesAction> OnResult);
		void GetAllConnectedNodes(std::function<void (std::vector<std::string>)> OnResult);

		//Load balancing
		int SetGeneralNodeIdSelectionAlgorithm(std::shared_ptr<OnMessageAlgorithmAction> algorithm);
		int SetGeneralNodeIdSelectionAlgorithm(std::function<std::string(const Message&)> algorithm);
		int SetTopicNodeIdSelectionAlgorithm(const std::string& pattern, const std::string& topic, std::shared_ptr<OnMessageAlgorithmAction> algorithm);
		int SetTopicNodeIdSelectionAlgorithm(const std::string& pattern, const std::string& topic, std::function<std::string(const Message&)> algorithm);

		void SetNodeCorrupted(const std::string& pattern, const std::string& topic, const std::string& node) {
			systemEventsManager.ReportNodeUnavaliable(pattern, topic, node);
		}

		virtual ~FrontEnd() {}

		std::string GetName();

		std::shared_ptr<BackEndConfiguration> GetExchangeContext() {
			return exchangeCtx;
		}

		std::shared_ptr<BackEndConfiguration> GetRunningContext() {
			return runningCtx;
		}

	};
}
#endif // !FRONTEND_HPP

