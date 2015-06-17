#include <MQCloud/CXX/Internal/Signals.hpp>
#include <MQCloud/CXX/Socket.hpp>
#include <MQCloud/CXX/BackEndConfiguration.hpp>
#include <map>

#ifndef CONNECTIONSHANDLER_HPP
#define CONNECTIONSHANDLER_HPP

namespace MQCloud {
	namespace Internal {
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
	}
}

#endif // !CONNECTIONSHANDLER_HPP
