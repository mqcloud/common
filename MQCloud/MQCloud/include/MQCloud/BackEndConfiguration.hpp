#include <MQCloud/PublishingSocket.hpp>
#include <MQCloud/SubscriberSocket.hpp>
#include <MQCloud/ThreadManagement.hpp>

#ifndef BACKENDCONFIGURATION_HPP
#define BACKENDCONFIGURATION_HPP

namespace MQCloud {
    struct BackEndConfiguration {
        std::shared_ptr<SubscriberSocketFactory> subscriberSocket;
        std::shared_ptr<PublishingSocketFactory> publishingSocket;

        std::string BackEndName; // BackEndName must include version only if one can not connect this backEnd to its older version

        virtual ~BackEndConfiguration() {
        }
    };
}

#endif // !BACKENDCONFIGURATION_HPP

