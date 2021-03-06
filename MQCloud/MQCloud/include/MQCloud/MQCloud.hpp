// Main Interface for C++ code and C#, Java and Python swig bindings 
// Note: C function pointers are implemented via expendable abstract classes with virtual functions for ease of swig binding.
// C++ API provides zero-copy, move and copy based approaches


//Note: all std::map can be replaced with folly or tbb analogs in future, current implementatiuon has only one dependency on protocol buffers. This topic shall be researched!

#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <map>

#include <MQCloud/Message.hpp>
#include <MQCloud/UserMessage.hpp>
#include <MQCloud/Protocol.pb.h>
#include <MQCloud/GenericFunc.hpp>
#include <MQCloud/GenericSignal.hpp>

#ifndef MQCloudCXX
#define MQCloudCXX

namespace MQCloud {
    namespace Internal {
    }
}
#endif // MQCloudCXX

