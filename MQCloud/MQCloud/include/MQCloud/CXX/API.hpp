#ifndef API_HPP
#define API_HPP
#include <MQCloud/CXX/BackEndConfiguration.hpp>
#include <MQCloud/CXX/FrontEnd.hpp>
#include <MQCloud/CXX/BackEndFactory.hpp>

#include <memory>


namespace MQCloud {

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

#endif // !API_HPP
