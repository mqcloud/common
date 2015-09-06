// Here we are using TBB. In case of slowdown (1 000 000+ handlers concurrently accessed ) one would use Folly AtomicHashMap.
// Note that Folly can be used for limited number of platforms!
#include <MQCloud/Delegates.hpp>
#include <tbb/concurrent_hash_map.h>
#include <mutex>
#include <memory>
#include <map>

#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

namespace MQCloud {
    namespace Internal {
        // For user on response handling
        struct ResponseHandler :
                OnUserMessageAction {
            tbb::concurrent_hash_map<int, std::shared_ptr<OnUserMessageAction>> handlers;

            virtual void OnAction(const UserMessage &m);

            void AddHandler(const int &id, std::shared_ptr<OnUserMessageAction> handler);

            void RemoveHandler(const int &id);

            void FreeAll();
        };

    }
}