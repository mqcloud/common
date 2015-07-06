#include <string>
#include <functional>
#include <MQCloud/Message.hpp>

#ifndef UserMessageHpp
#define UserMessageHpp

namespace MQCloud {
    struct UserMessage :
            protected Message {
        UserMessage(const Message &m) :
                Message(m) {
        }

        void SetData(const std::string &_data) {
            data = _data;
        }

        const std::string &GetData() const {
            return data;
        }

        void SetTopic(const std::string &_data) {
            topic = _data;
        }

        const std::string &GetTopic() const {
            return topic;
        }

        std::string &GetSender() {
            return serviceId;
        }

        int GetMessageId() const {
            return GetMessageId();
        }
    };
}
#endif // UserMessageHpp

