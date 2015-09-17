#ifndef CONNECTIONSHANDLER_HPP
#define CONNECTIONSHANDLER_HPP

#include <MQCloud/Internal/Signals.hpp>
#include <MQCloud/Socket.hpp>
#include <MQCloud/BackEndConfiguration.hpp>
#include <MQCloud/Internal/PatternTopicResponseHandler.hpp>

#include <TBB/concurrent_hash_map.h>

namespace MQCloud {
    namespace Internal {
        struct ConnectionsHandler {
        private:
            int                                            HeartBeatRate;

            std::shared_ptr<BackEndConfiguration>          ctx;
            std::shared_ptr<Socket>                        Out;
            std::shared_ptr<Socket>                        subscribingSocket;
            std::string                                    serviceId;

            tbb::concurrent_hash_map<std::string, std::shared_ptr<SocketConnection>> subscribtionCnnections;

            std::shared_ptr<GeneralMessageHandler>         inHandler;
            std::shared_ptr<StaticResponseHandler>         patternTopicPairHandler;

        public:
            std::shared_ptr<GeneralStringHandler> OnExchangeError;
            std::shared_ptr<GeneralStringHandler> OnDisconnected;
            std::shared_ptr<GeneralStringHandler> OnNodeDisconectedFromThis;
            std::shared_ptr<GeneralStringHandler> OnThisNodeDisconectedFromOther;

        private:
            void OnExternalNodeSocketDisconnect(const std::string &error) {
                //TODO Clear handlers
            }

            void OnHostNodeSocketDisconnect(const std::string &error) {
                //TODO Exchange Error, close up
            }

        public:

            ConnectionsHandler(std::shared_ptr<BackEndConfiguration> ctx);

            void RemoveNode(const std::string &addr);

            void ConnectToExchangeNode(const std::string &addr, std::shared_ptr<OnError> onError);

            void ConnectToNode(const std::string & addr);

            void DisconnectFromNode(const std::string & addr) {
                // ToDo implement!

                OnThisNodeDisconectedFromOther->OnAction(addr);
            }

            //Global
            void AddOnMesageHandler(std::shared_ptr<OnMessageAction> handler);

            //Filtered
            void AddOnMesageHandler(const std::string &pattern,
                                                const std::string &topic,
                                                std::shared_ptr<OnUserMessageAction> handler);

            void PublishMessage(Message &m);

            void SetServiceId(const std::string &id);

            void SetHeartBeatRate(const int &heartbeatratems);

            int GetHeartBeatRate() const;

			std::string getOutgoingUrl() {
				return Out->SocketId;
			}
        };
    }
}

#endif // !CONNECTIONSHANDLER_HPP

