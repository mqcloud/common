//
// Created by Oleg Jakushkin on 29.07.15.
//

#include <MQCloud/Internal/ConnectionsHandler.hpp>

int MQCloud::Internal::ConnectionsHandler::GetHeartBeatRate() const {
    return HeartBeatRate;
}

void MQCloud::Internal::ConnectionsHandler::SetHeartBeatRate(const int &heartbeatratems) {
    HeartBeatRate = heartbeatratems;
}

void MQCloud::Internal::ConnectionsHandler::SetServiceId(const std::string &id) {
    serviceId = id;
}

void MQCloud::Internal::ConnectionsHandler::PublishMessage(Message &m) {
    m.serviceId = serviceId;
    ctx->PublishingSocket->PublishMessage(Out, m);
}

void MQCloud::Internal::ConnectionsHandler::AddOnMesageHandler(const std::string &pattern,
                                                               const std::string &topic,
                                                               std::shared_ptr<OnUserMessageAction> handler) {
    patternTopicPairHandler->AddHandler(pattern, topic, handler);
}

void MQCloud::Internal::ConnectionsHandler::AddOnMesageHandler(std::shared_ptr<OnMessageAction> handler) {
    inHandler->AddHandler(handler);
}

void MQCloud::Internal::ConnectionsHandler::ConnectToNode(const std::string & addr) {
    auto                  connection        = ctx->SubscriberSocket
                                                 ->ConnectSubscribingSocket(subscribingSocket, addr,
                                                                            inHandler);
    auto                  id                = connection->SocketId;
    std::weak_ptr<Socket> weekConnectionPtr = connection;

    auto handler = std::make_shared<OnError>([&, weekConnectionPtr, id, addr] {
        if (auto weekConnectionSrc = weekConnectionPtr.lock()) {
            tbb::concurrent_hash_map<int, std::shared_ptr<Socket>>::accessor lambdaAccessor;
            if (subscribtionCnnections.find(lambdaAccessor, id)) {
                subscribtionCnnections.erase(lambdaAccessor);
            }
        }
        OnNodeDisconectedFromThis->OnAction(addr);
    });

    connection->AddDisconnectHandler(handler);
    connection->Connect();

    tbb::concurrent_hash_map<int, std::shared_ptr<Socket>>::accessor instance;

    subscribtionCnnections.insert(instance, id);
    instance->second = connection;
}

void MQCloud::Internal::ConnectionsHandler::ConnectToExchangeNode(const std::string &addr, std::shared_ptr<OnError> onError) {
    auto                  connection        = ctx->SubscriberSocket
                                                 ->ConnectSubscribingSocket(subscribingSocket, addr,
                                                                            inHandler);
    auto                  id                = connection->SocketId;
    std::weak_ptr<Socket> weekConnectionPtr = connection;
    OnExchangeError->AddHandler(onError);

    auto handler = std::make_shared<OnError>([&, weekConnectionPtr, id, addr](const std::string &error) {
        if (auto weekConnectionSrc = weekConnectionPtr.lock()) {
            tbb::concurrent_hash_map<int, std::shared_ptr<Socket>>::accessor lambdaAccessor;
            if (subscribtionCnnections.find(lambdaAccessor, id)) {
                subscribtionCnnections.erase(lambdaAccessor);
            }
        }
        OnExchangeError->OnAction(error);
        OnNodeDisconectedFromThis->OnAction(addr);
    });

    connection->AddDisconnectHandler(handler);
    connection->Connect();

    tbb::concurrent_hash_map<int, std::shared_ptr<Socket>>::accessor instance;

    subscribtionCnnections.insert(instance, id);
    instance->second = connection;
}

void MQCloud::Internal::ConnectionsHandler::RemoveNode(const std::string &addr) {
    subscribtionCnnections.erase(addr);
}

MQCloud::Internal::ConnectionsHandler::ConnectionsHandler(std::shared_ptr<BackEndConfiguration> ctx) :
        ctx(ctx),
        serviceId("-1"),
        inHandler(std::make_shared<GeneralMessageHandler>()),
        patternTopicPairHandler(std::make_shared<StaticResponseHandler>()) {

    OnDisconnected->AddHandler(std::make_shared<OnError>([&](const std::string &error) {
        subscribtionCnnections.clear();
    }));

    OnNodeDisconectedFromThis->AddHandler(std::make_shared<OnError>([&](const std::string &error) {
        OnExternalNodeSocketDisconnect(error);
    }));

    OnThisNodeDisconectedFromOther->AddHandler(std::make_shared<OnError>([&](const std::string &error) {
        OnHostNodeSocketDisconnect(error);
    }));

    Out = ctx->PublishingSocket->CreatePublishingSocket();
    Out->AddDisconnectHandler(OnDisconnected);
    Out->Connect();

    subscribingSocket = ctx->SubscriberSocket->CreateSubscribingSocket();
    subscribingSocket->AddDisconnectHandler(OnDisconnected);
    subscribingSocket->Connect();

    AddOnMesageHandler(patternTopicPairHandler);
}