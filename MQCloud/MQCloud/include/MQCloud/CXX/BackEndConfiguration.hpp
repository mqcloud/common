#include <MQCloud/CXX/PublishingSocket.hpp>
#include <MQCloud/CXX/SubscriberSocket.hpp>
#include <MQCloud/CXX/ThreadManagement.hpp>

#ifndef BACKENDCONFIGURATION_HPP
#define BACKENDCONFIGURATION_HPP

namespace MQCloud {
	struct BackEndConfiguration {
		std::shared_ptr<ThreadManagementInterface> ThreadingInterface;
		std::shared_ptr<SubscriberSocketInterface> SubscriberSocketInterface;
		std::shared_ptr<PublishingSocketInterface> PublishingSocketInterface;

		std::string BackEndName; // BackEndName must include version only if one can not connect this backEnd to its older version

		virtual ~BackEndConfiguration() {}
	};
}

#endif // !BACKENDCONFIGURATION_HPP

