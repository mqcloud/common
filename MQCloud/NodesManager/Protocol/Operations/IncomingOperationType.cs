using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public enum IncomingOperationType {
        ConnectRequest,
        GetPublisherRequest,
        GetAllPublishersRequest,
        GetSubscriberRequest,
        GetAllSubscribersRequest
    }
}