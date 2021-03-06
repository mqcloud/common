#ifndef MESSAGEHPP
#define MESSAGEHPP

#include <atomic>
#include <string>

namespace MQCloud {
    enum MessageState {
        MessageStateOk = 0, MessageStateError = 1,
    };

    struct Message {
        std::string topic;
        std::string data;
        std::string serviceId;
        std::string PatternName;
        std::string PatternData;

        MessageState State;
        std::string  ErrorData;

        Message() {
            SetMessageId();
        }

        Message(const std::string &data,
                const std::string &topic,
                const std::string &pattern,
                const std::string &patternData) :
                topic(topic), data(data), serviceId(serviceId), PatternName(pattern), PatternData(patternData) {
					SetMessageId();
		}

        Message( int id,
                const std::string &data,
                const std::string &topic,
                const std::string &pattern,
                const std::string &patternData) :
                topic(topic), data(data), serviceId(serviceId), PatternName(pattern), PatternData(patternData), id(id) {}
		
		Message( int id,
                const std::string &data,
                const std::string &topic,
                const std::string &pattern,
                const std::string &patternData,
				const MessageState State, 
				const std::string &ErrorData) :
                topic(topic), data(data), serviceId(serviceId), PatternName(pattern), PatternData(patternData), id(id), State(State), ErrorData(ErrorData) { }

        int GetMessageId() const {
            return id;
        }

        void SetMessageId() {
            static std::atomic<int> gid = 0;
            id = gid.fetch_add(1);
        }


    private:
        int id;
    };
}
#endif // MESSAGEHPP

