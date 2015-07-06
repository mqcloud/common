#include <MQCloud/Internal/ConnectionsHandler.hpp>
#include <MQCloud/Internal/ResponseHandler.hpp>
#include <MQCloud/Internal/MessagesManager.hpp>
#include <MQCloud/Internal/PatternTopicResponseHandler.hpp>
#include <MQCloud/Protocol.pb.h>

#ifndef EXCHANGEOPERATIONSMANAGER_HPP
#define EXCHANGEOPERATIONSMANAGER_HPP

namespace MQCloud {
    namespace Internal {
        struct ExchangeOperationsManager :
                MessagesManager {
            const int operationsSocketId = 0;

            // _SO
            const std::string serverPatternOperation;

            // _SG
            const std::string serverTopicNodeOperation;

            std::string runningBackEndName;

            std::shared_ptr<ResponseHandler> responseHandler;

            explicit ExchangeOperationsManager(std::shared_ptr<ConnectionsHandler> ctx,
                                               const std::string &runningBackEndName) :
                    MessagesManager(ctx),
                    serverPatternOperation("_SPO"),
                    serverTopicNodeOperation("_SO"),
                    responseHandler(std::make_shared<ResponseHandler>()) {
                ctx->AddOnMesageHandler(serverPatternOperation, serverTopicNodeOperation, responseHandler);
            }

            void ConnectRequest(const std::string &desiredServiceName, std::shared_ptr<OnConnectedAction> handler) {
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

                auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage &m) {
                    Protocol::IncomingOperation incomingOperation;
                    incomingOperation.ParseFromString(m.GetData());

                    const auto &response(incomingOperation.connectresponse());

                    ctx->SetServiceId(response.realnodename());
                    ctx->SetHeartBeatRate(response.heartbeatratems());

                    handler->OnAction(response.realnodename());
                });
                responseHandler->AddHandler(result.GetMessageId(), action);
                ctx->PublishMessage(result);
            }

            void ConnectRequest(const std::string &desiredServiceName, std::function<std::string> handler) {
                ConnectRequest(desiredServiceName, std::make_shared<OnConnectedAction>(handler));
            }

            void PrepareMessage(Message &result) {
                result.PatternName = serverPatternOperation;
                result.topic       = serverTopicNodeOperation;
            }

            void GetAllPublishingNodes(const std::string &pattern,
                                       const std::string &topic,
                                       std::shared_ptr<OnNodesAction> handler) {
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

                auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage &m) {
                    Protocol::IncomingOperation incomingOperation;
                    incomingOperation.ParseFromString(m.GetData());

                    const auto &response(incomingOperation.getallpublishersresponse());

                    std::vector<const std::string &> vec(response.nodes().begin(), response.nodes().end());
                    handler->OnAction(vec);
                });

                responseHandler->AddHandler(result.GetMessageId(), action);
                ctx->PublishMessage(result);
            }

            void GetAllPublishingNodes(const std::string &pattern,
                                       const std::string &topic,
                                       std::function<std::vector<const std::string &>> handler) {
                GetAllPublishingNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
            }

            void GetPublisherNode(const std::string &pattern,
                                  const std::string &topic,
                                  std::shared_ptr<OnNodeAction> handler) {
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

                auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage &m) {
                    Protocol::IncomingOperation incomingOperation;
                    incomingOperation.ParseFromString(m.GetData());

                    const auto &response(incomingOperation.getpublisherresponse());

                    handler->OnAction(response.node());
                });

                responseHandler->AddHandler(result.GetMessageId(), action);
                ctx->PublishMessage(result);
            }

            void GetPublisherNode(const std::string &pattern,
                                  const std::string &topic,
                                  std::function<const std::string &> handler) {
                GetPublisherNode(pattern, topic, std::make_shared<OnNodeAction>(move(handler)));
            }

            void GetSubscriberNode(const std::string &pattern,
                                   const std::string &topic,
                                   std::shared_ptr<OnNodeAction> handler) {
                Protocol::GetSubscriberRequest request;
                request.set_pattern(pattern);
                request.set_topic(topic);

                Protocol::OutgoingOperation operation;

                operation.set_typecode(Protocol::OutgoingOperationTypeGetSubscriberRequest);
                operation.set_allocated_getsubscriberrequest(&request);

                auto    serialized = operation.SerializeAsString();
                Message result;
                result.data = serialized;
                PrepareMessage(result);

                auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage &m) {
                    Protocol::IncomingOperation incomingOperation;
                    incomingOperation.ParseFromString(m.GetData());

                    const auto &response(incomingOperation.getsubscriberresponse());
                    handler->OnAction(response.node());
                });

                responseHandler->AddHandler(result.GetMessageId(), action);
                ctx->PublishMessage(result);
            }

            void GetSubscriberNode(const std::string &pattern,
                                   const std::string &topic,
                                   std::function<const std::string &> handler) {
                GetSubscriberNode(pattern, topic, std::make_shared<OnNodeAction>(move(handler)));
            }

            void GetAllSubscribedNodes(const std::string &pattern,
                                       const std::string &topic,
                                       std::shared_ptr<OnNodesAction> handler) {
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

                auto action = std::make_shared<OnUserMessageAction>([=](const UserMessage &m) {
                    Protocol::IncomingOperation incomingOperation;
                    incomingOperation.ParseFromString(m.GetData());

                    const auto &response(incomingOperation.getallsubscribersresponse());

                    std::vector<const std::string &> vec(response.nodes().begin(), response.nodes().end());
                    handler->OnAction(vec);
                });

                responseHandler->AddHandler(result.GetMessageId(), action);
                ctx->PublishMessage(result);
            }

            void GetAllSubscribedNodes(const std::string &pattern,
                                       const std::string &topic,
                                       std::function<std::vector<const std::string &>> handler) {
                GetAllSubscribedNodes(pattern, topic, std::make_shared<OnNodesAction>(move(handler)));
            }

        };
    }
}

#endif // !EXCHANGEOPERATIONSMANAGER_HPP

