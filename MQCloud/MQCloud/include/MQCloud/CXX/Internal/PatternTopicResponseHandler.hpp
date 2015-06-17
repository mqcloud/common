#include <MQCloud/CXX/Delegates.hpp>
#include <mutex>
#include <memory>
#include <map>


#ifndef PATTERNTOPICRESPONSEHANDLER_HPP
#define PATTERNTOPICRESPONSEHANDLER_HPP

namespace MQCloud {
	namespace Internal {
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
	}
}

#endif // !PATTERNTOPICRESPONSEHANDLER_HPP
