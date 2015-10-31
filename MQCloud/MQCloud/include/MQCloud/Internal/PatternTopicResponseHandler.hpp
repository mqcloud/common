#include <MQCloud/Delegates.hpp>
#include <mutex>
#include <memory>
#include <unordered_map>


#ifndef PATTERNTOPICRESPONSEHANDLER_HPP
#define PATTERNTOPICRESPONSEHANDLER_HPP

namespace MQCloud {
    namespace Internal {
        // For user on request handling
        struct StaticResponseHandler :
                OnMessageAction {
            std::unordered_map<std::string,  std::unordered_map<std::string, std::shared_ptr<OnMessageAction>>> handlers;
            std::mutex                                              mutex;

            virtual void OnAction(const Message &m) {
                std::unique_lock<std::mutex> lockHandlers(mutex);
                lockHandlers.lock();
                auto it_pattern = handlers.find(m.PatternName);
                if (it_pattern != handlers.end()) {
					auto it_topic = it_pattern->second.find(m.PatternName);
					if (it_topic != it_pattern->second.end()) {
						auto action = it_topic->second;
						lockHandlers.unlock();
						action->OnAction(m);
					} else {
						lockHandlers.unlock();
					}
                } else {
					lockHandlers.unlock();
				}
            }

            void AddHandler(const std::string &pattern,
                            const std::string &topic,
                            std::shared_ptr<OnMessageAction> handler) {
                std::lock_guard<std::mutex> lockHandlers(mutex);
                handlers[pattern][topic] = handler;
            }

            void RemoveHandler(const std::string &pattern, const std::string &topic) {
                std::lock_guard<std::mutex> lockHandlers(mutex);
                auto                        it_pattern = handlers.find(pattern);
                if (it_pattern != handlers.end()) {
					auto it_topic = it_pattern->second.find(topic);
					if(it_topic != it_pattern->second.end()) {
						it_pattern->second.erase(it_topic);
						if(it_pattern->second.size() == 0) {
							handlers.erase(it_pattern);
						}
					}
                }
            }
        };
    }
}

#endif // !PATTERNTOPICRESPONSEHANDLER_HPP

