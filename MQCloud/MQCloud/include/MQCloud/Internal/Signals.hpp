#ifndef INTERNAL_SIGNALS_HPP
#define INTERNAL_SIGNALS_HPP

#include <MQCloud/GenericSignal.hpp>
#include <MQCloud/Message.hpp>

namespace MQCloud {
    namespace Internal {
        struct GeneralMessageHandler :
                GenericSignalHandler<const Message &> {
        };

        struct GeneralStringHandler :
                GenericSignalHandler<const std::string &> {
        };

        struct GeneralStringStringHandler :
                GenericSignalHandler<const std::string &, const std::string &> {
        };

        struct GeneralTaskHandler :
                GenericSignalHandler<void> {
        };
    }
}

#endif // !INTERNAL_SIGNALS_HPP

