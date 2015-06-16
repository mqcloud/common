using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public enum OutgoingOperationType {
        ConnectRequest,
        GetPublisherRequest,
        GetAllPublishersRequest,
        GetSubscriberRequest,
        GetAllSubscribersRequest
    }
}