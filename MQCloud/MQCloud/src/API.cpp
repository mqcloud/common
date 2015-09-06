//
// Created by Oleg Jakushkin on 12.07.15.
//
#include <MQCloud/API.hpp>

MQCloud::API::API(std::shared_ptr < ::MQCloud::BackEndFactory > runningBackEnd,
                  std::shared_ptr < ::MQCloud::BackEndConfiguration > exchangeCtx) {
    frontEnd = std::make_shared<FrontEnd>(exchangeCtx, runningBackEnd->CreateContext());
}

MQCloud::API::API(std::shared_ptr<BackEndFactory> runningBackEnd, const std::string &exchengeAdress) {
    frontEnd = std::make_shared<FrontEnd>(runningBackEnd, exchengeAdress);
}

void MQCloud::API::Connect(const std::string &desiredName, std::shared_ptr<OnConnectedAction> handler) {
    frontEnd->Connect(desiredName, handler);
}

std::string MQCloud::API::GetName() {
    return frontEnd->GetName();
}

std::shared_ptr<FrontEnd> MQCloud::API::GetFrontEnd() {
    return frontEnd;
}

void MQCloud::API::Request(const UserMessage &out, std::shared_ptr<OnUserMessageAction> OnMessage) {
    auto message = (MQCloud::Message *) &out;
    message->PatternName = "_RR";
    frontEnd->RegisterResponseCalback("_RR", message->topic, message->GetMessageId(), [=](const MQCloud::Message &in) {
        auto out = UserMessage(in);
        OnMessage->OnAction(out);
    });
    frontEnd->PublishMessageToAnyNode(*message);

}

void MQCloud::API::RequestTarget(const UserMessage &out,
                                 const std::string &target,
                                 std::shared_ptr<OnUserMessageAction> OnMessage) {
    auto message = (MQCloud::Message *) &out;
    message->PatternName = "_RR";
    frontEnd->RegisterResponseCalback("_RR", message->topic, message->GetMessageId(), [=](const Message &in) {
        auto out = UserMessage(in);
        OnMessage->OnAction(out);
    });
    frontEnd->PublishMessageToNode(target, *message);
}

void MQCloud::API::AdvertiseRepliesOnTopic(const std::string &topic, std::shared_ptr<OnUserMessageAction> OnMessage) {
    frontEnd->Subscribe("_RR", topic, std::make_shared<OnNodesAction>(), [=](const Message &m) {
        auto message = UserMessage(m);
        OnMessage->OnAction(message);
    });
}

void MQCloud::API::AdvertizeRequestsOnTopic(const std::string &topic) {
    frontEnd->AdvertiseTopic("_RR", topic);
    frontEnd->RegisterResponsesHandler("_RR", topic);
}

void MQCloud::API::CloseRequestsOnTopic(const std::string &topic) {
    frontEnd->AdvertiseTopic("_RR", topic);
}

void MQCloud::API::CloseReplysOnTopic(const std::string &topic) {
    frontEnd->Unsubscribe("RR", topic);
}

void MQCloud::API::AdvertizePublishingOnTopic(const std::string &topic) {
    frontEnd->AdvertiseTopic("_PS", topic);
}

void MQCloud::API::PublishMessage(const UserMessage &out) {
    auto message = (MQCloud::Message *) &out;
    message->PatternName = "_PS";
    frontEnd->PublishMessageToAllNodes(*message);
}

void MQCloud::API::ClosePublishingOnTopic(const std::string &topic) {
    frontEnd->RejectTopic("_PS", topic);
}

void MQCloud::API::Subscribe(const std::string &topic, std::shared_ptr<OnUserMessageAction> OnMessage) {
    frontEnd->Subscribe("_PS", topic,
                        [=] (std::vector<const std::string &>){},
                        [=](const MQCloud::Message &m) {
        auto message = UserMessage(m);
        OnMessage->OnAction(message);
    });
}

void MQCloud::API::UnSubscribe(const std::string &topic) {
    frontEnd->Unsubscribe("_PS", topic);
}

void MQCloud::API::RequestMany(int MaxRespondents,
                               int timeout,
                               const UserMessage &out,
                               std::shared_ptr<OnUserMessagesAction> OnReplyFromMany) {
    auto message = (MQCloud::Message *) &out;
    message->PatternName = "_RR";
    frontEnd->GetAllSubscribedNodes("_RR", message->topic, [=](std::vector<std::string> r) {
        if (r.size() > MaxRespondents) {
            std::random_shuffle(r.begin(), r.end());
            r.resize(MaxRespondents);
        }
        frontEnd->PublishMessageToNodes(r, *message);
        frontEnd->RegisterMultipleResponsesCalback(std::string("_RR"), message->topic, message->GetMessageId(),
                                                   timeout, [=](std::vector<Message> in) {
                    std::vector<UserMessage> m;
                    m.reserve(in.size());
                    m.assign(in.begin(), in.end());
                    OnReplyFromMany->OnAction(m);
                });
    });
}