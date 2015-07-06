#include <string>

#ifndef EVENTSHANDLER_HPP
#define EVENTSHANDLER_HPP

namespace MQCloud {
	//for end users
	struct EventsHandler {
		virtual void OnExchangeError(const std::string& error);
		virtual void OnConnected();
		virtual void OnDisconnected();
		virtual void OnNewSubscriber(const std::string& topic, const std::string& subscriber);

		virtual ~EventsHandler() {}
	};
}

#endif // !EVENTSHANDLER_HPP

