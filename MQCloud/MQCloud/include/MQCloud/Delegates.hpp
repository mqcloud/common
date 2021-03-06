#include <MQCloud/Message.hpp>
#include <MQCloud/GenericFunc.hpp>
#include <MQCloud/UserMessage.hpp>

#include <vector>

#ifndef DELEGATES_HPP
// Delegates (lambda functions constructable/swig %template definable for OnAction overloading)
namespace MQCloud {
    typedef GenericAction<>                                 Task;
    typedef GenericAction<const Message &>                  OnMessageAction;
    typedef GenericAction<std::vector<Message>>             OnMessagesAction;
    typedef GenericAction<const UserMessage &>              OnUserMessageAction;
    typedef GenericAction<std::vector<UserMessage>>         OnUserMessagesAction;
    typedef GenericAction<std::vector<const std::string &>> OnNodesAction; // Gets a vector of ServiceId's
    typedef GenericAction<const std::string &>              OnNodeAction; // Get a ServiceId
    typedef GenericAction<std::string>                      OnConnectedAction;
    typedef GenericAction<>                                 OnDeclaredOffline;
    typedef GenericAction<const std::string &>              OnError;

    typedef GenericFunc<std::string, const MQCloud::Message &> OnMessageAlgorithmAction; // returns ServiceId to send to
}
#endif // !DELEGATES_HPP

