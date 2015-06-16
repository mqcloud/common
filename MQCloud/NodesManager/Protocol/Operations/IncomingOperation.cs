using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class IncomingOperation {
        [ProtoMember(2)]
        public ConnectResponse ConnectResponse;

        [ProtoMember(4)]
        public GetAllPublishersResponse GetAllPublishersResponse;

        [ProtoMember(6)]
        public GetAllSubscribersResponse GetAllSubscribersResponse;

        [ProtoMember(3)]
        public GetPublisherResponse GetPublisherResponse;

        [ProtoMember(5)]
        public GetSubscriberResponse GetSubscriberResponse;

        [ProtoMember(1)]
        public IncomingOperationType TypeCode;
    }
}