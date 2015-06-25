#ifndef API_HPP
#define API_HPP
#include <MQCloud/CXX/BackEndConfiguration.hpp>
#include <MQCloud/CXX/FrontEnd.hpp>
#include <MQCloud/CXX/BackEndFactory.hpp>

#include <memory>

// remove UserMassage use from all internal handlers where patterns may be relevant
// use connection in FrontEnd API
// imlement API
// test against ZeroMQ
// test against IB

namespace MQCloud {
	// Throws if can not connect to the Exchange
	struct API {
	private:
		std::shared_ptr<FrontEnd> frontEnd;
	public:
		API(std::shared_ptr<BackEndFactory> runningBackEnd, std::shared_ptr<BackEndConfiguration> exchangeCtx) {
			frontEnd = std::make_shared<FrontEnd>(exchangeCtx, runningBackEnd->CreateContext());
		}

		API(std::shared_ptr<BackEndFactory> runningBackEnd, const std::string & exchengeAdress) {
			frontEnd = std::make_shared<FrontEnd>(runningBackEnd, exchengeAdress);
		}

		void Connect(const std::string & desiredName, std::shared_ptr<OnConnectedAction> handler) {
			frontEnd->Connect(desiredName, handler);
		}

		std::string GetName() {
			return frontEnd->GetName();
		}

		std::shared_ptr<FrontEnd> GetFronEnd() {
			return frontEnd;
		}

		// Request-Reply, Request-Reply continuos *(nonblocking)
		void Request(const UserMessage & out, std::shared_ptr<OnUserMessageAction> OnMessage) {
			auto message = (Message*)&out;
			message->PatternName = "_RR";
			frontEnd->RegisterResponseCalback("_RR", message->topic, message->GetMessageId(),
			                                  [=](const Message & in) {
				                                  auto out = UserMessage(in);
				                                  OnMessage->OnAction(out);
			                                  });
			frontEnd->PublishMessageToAnyNode(*message);

		}

		void RequestTarget(const UserMessage & out, const std::string & target, std::shared_ptr<OnUserMessageAction> OnMessage) {
			auto message = (Message*)&out;
			message->PatternName = "_RR";
			frontEnd->RegisterResponseCalback("_RR", message->topic, message->GetMessageId(),
			                                  [=](const Message & in) {
				                                  auto out = UserMessage(in);
				                                  OnMessage->OnAction(out);
			                                  });
			frontEnd->PublishMessageToNode(target, *message);
		}

		void AdvertizeReplysOnTopic(const std::string & topic, std::shared_ptr<OnUserMessageAction> OnMessage) {
			frontEnd->Subscribe("_RR", topic, std::make_shared<OnNodesAction>(), [=](const Message & m) {
				                    auto message = UserMessage(m);
				                    OnMessage->OnAction(message);
			                    });
		}

		void AdvertizeRequestsOnTopic(const std::string & topic) {
			frontEnd->AdvertiseTopic("_RR", topic);
			frontEnd->RegisterResponsesHandler("_RR", topic);
		}

		void CloseRequestsOnTopic(const std::string & topic) {
			frontEnd->AdvertiseTopic("_RR", topic);
		}

		void CloseReplysOnTopic(const std::string & topic) {
			frontEnd->Unsubscribe("RR", topic);
		}

		// Publish-Subscribe
		void AdvertizePublishingOnTopic(const std::string & topic) {
			frontEnd->AdvertiseTopic("_PS", topic);
		}

		void PublishMessage(const UserMessage & out) {
			auto message = (Message*)&out;
			message->PatternName = "_PS";
			frontEnd->PublishMessageToAllNodes(*message);
		}

		void ClosePublishingOnTopic(const std::string & topic) {
			frontEnd->RejectTopic("_PS", topic);
		}

		void Subscribe(const std::string & topic, std::shared_ptr<OnUserMessageAction> OnMessage) {
			frontEnd->Subscribe("_PS", topic, [=](const Message & m) {
				                    auto message = UserMessage(m);
				                    OnMessage->OnAction(message);
			                    });
		}

		void UnSubscribe(const std::string & topic) {
			frontEnd->Unsubscribe("_PS", topic);
		}

		// Survey-Respondent *(nonblocking, nonforcing)
		void RequestMany(int MaxRespondents, int timeout, const UserMessage & out, std::shared_ptr<OnUserMessagesAction> OnReplyFromMany) {
			auto message = (Message*)&out;
			message->PatternName = "_RR";
			frontEnd->GetAllSubscribedNodes("_RR", message->topic, [=](std::vector<std::string> r) {
				                                if(r.size() > MaxRespondents) {
					                                std::random_shuffle(r.begin(), r.end());
					                                r.resize(MaxRespondents);
				                                }
				                                frontEnd->PublishMessageToNodes(r, message);
				                                frontEnd->RegisterMultipleResponsesCalback(std::string("_RR"), message->topic, message->GetMessageId(), timeout,
				                                                                           [=](std::vector<Message> in) {
					                                                                           std::vector<UserMessage> m;
					                                                                           m.reserve(in.size());
					                                                                           m.assign(in.begin(), in.end());
					                                                                           OnReplyFromMany->OnAction(m);
				                                                                           });
			                                });
		}
	};
}

#endif // !API_HPP

