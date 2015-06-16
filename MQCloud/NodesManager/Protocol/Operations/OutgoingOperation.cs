using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class OutgoingOperation {
        [ProtoMember(2)]
        public ConnectRequest ConnectRequest;

        [ProtoMember(4)]
        public GetAllPublishersRequest GetAllPublishersRequest;

        [ProtoMember(6)]
        public GetAllSubscribersRequest GetAllSubscribersRequest;

        [ProtoMember(3)]
        public GetPublisherRequest GetPublisherRequest;

        [ProtoMember(5)]
        public GetSubscriberRequest GetSubscriberRequest;

        [ProtoMember(1)]
        public OutgoingOperationType TypeCode;
    }
}