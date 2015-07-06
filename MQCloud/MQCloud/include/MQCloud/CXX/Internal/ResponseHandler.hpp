#include <MQCloud/CXX/Delegates.hpp>
#include <mutex>
#include <memory>
#include <map>

#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

namespace MQCloud {
	namespace Internal {
		// For user on response handling
		struct ResponseHandler :  OnUserMessageAction {
			std::map<int, std::shared_ptr<OnUserMessageAction>> handlers;
			std::mutex mutex;

			virtual void OnAction(const UserMessage& m) {
				auto id = m.GetMessageId();

				std::unique_lock<std::mutex> lockHandlers(mutex);
				lockHandlers.lock();
				auto it = handlers.find(id);
				if(it != handlers.end()) {
					auto action = it->second;
					handlers.erase(it);
					lockHandlers.unlock();
					action->OnAction(m);
				}
			}

			void AddHandler(const int& id, std::shared_ptr<OnUserMessageAction> handler) {
				std::lock_guard<std::mutex> lockHandlers(mutex);
				handlers[id] = handler;
			}

			void RemoveHandler(const int& id) {
				std::lock_guard<std::mutex> lockHandlers(mutex);
				auto it = handlers.find(id);
				if(it != handlers.end()) {
					handlers.erase(it);
				}
			}
		};
	}
}

#endif // !RESPONSEHANDLER_HPP

