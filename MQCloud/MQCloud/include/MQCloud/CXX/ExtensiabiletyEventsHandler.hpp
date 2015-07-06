#include <MQCloud/CXX/Message.hpp>

#ifndef EXTENSIABILETYEVENTSHANDLER_HPP
#define EXTENSIABILETYEVENTSHANDLER_HPP

namespace MQCloud {
	//For pattern developers
	struct ExtensiabiletyEventsHandler {

		virtual void OnMessage(const Message& in);

		virtual void OnNodeConnectedToThis(const std::string& addr);
		virtual void OnNodeConnectedToOther(const std::string& addr);

		virtual void OnNodeAdvertisedTopic(const std::string& pattern, const std::string& topic, const std::string& node);
		virtual void OnNodeRejectedTopic(const std::string& pattern, const std::string& topic, const std::string& node);

		virtual void OnNodeSubscribedToTopic(const std::string& pattern, const std::string& topic, const std::string& node);
		virtual void OnNodeUnsubscribedFromTopic(const std::string& pattern, const std::string& topic, const std::string& node);

		virtual ~ExtensiabiletyEventsHandler() {}
	};
}
#endif // !EXTENSIABILETYEVENTSHANDLER_HPP

