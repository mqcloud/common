#include <MQCloud/CXX/PublishingSocket.hpp>
#include <MQCloud/CXX/SubscriberSocket.hpp>
#include <MQCloud/CXX/ThreadManagement.hpp>

#ifndef BACKENDCONFIGURATION_HPP
#define BACKENDCONFIGURATION_HPP

namespace MQCloud {
	
		struct CoreConfiguration {
		CoreThreadManagement ThreadingInterface;
		CoreSubscriberSocket SubscriberSocketInterface;
		CorePublishingSocket PublishingSocketInterface;

		std::string BackEndName; // BackEndName must include version only if one can not connect this backEnd to its older version

		virtual ~CoreConfiguration() {}
	};
}

#endif // !BACKENDCONFIGURATION_HPP
