#ifndef API_HPP
#define API_HPP

#include <MQCloud/BackEndConfiguration.hpp>
#include <MQCloud/FrontEnd.hpp>
#include <MQCloud/BackEndFactory.hpp>

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
        API(std::shared_ptr<BackEndFactory> runningBackEnd, std::shared_ptr<BackEndConfiguration> exchangeCtx);

        API(std::shared_ptr<BackEndFactory> runningBackEnd, const std::string &exchengeAdress);

        void Connect(const std::string &desiredName, std::shared_ptr<OnConnectedAction> handler);

        std::string GetName();

        std::shared_ptr<FrontEnd> GetFrontEnd();

        void Request(const UserMessage &out, std::shared_ptr<OnUserMessageAction> OnMessage);

        void RequestTarget(const UserMessage &out,
                                   const std::string &target,
                                   std::shared_ptr<OnUserMessageAction> OnMessage);

        void AdvertiseRepliesOnTopic(const std::string &topic, std::shared_ptr<OnUserMessageAction> OnMessage);

        void AdvertizeRequestsOnTopic(const std::string &topic);

        void CloseRequestsOnTopic(const std::string &topic);

        void CloseReplysOnTopic(const std::string &topic);

        void AdvertizePublishingOnTopic(const std::string &topic);
			
        void PublishMessage(const UserMessage &out);

        void ClosePublishingOnTopic(const std::string &topic);

        void Subscribe(const std::string &topic, std::shared_ptr<OnUserMessageAction> OnMessage);

        void UnSubscribe(const std::string &topic);

        void RequestMany(int MaxRespondents,
                                 int timeout,
                                 const UserMessage &out,
                                 std::shared_ptr<OnUserMessagesAction> OnReplyFromMany);
    };
}

#endif // !API_HPP

