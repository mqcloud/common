using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class OutgoingOperation {
        [ProtoMember( 1 )]
        public OutgoingOperationType TypeCode;

        [ProtoMember(2)]
        public ConnectRequest ConnectRequest;
        [ProtoMember( 3 )]
        public GetPublisherRequest GetPublisherRequest;
        [ProtoMember( 4)]
        public GetAllPublishersRequest GetAllPublishersRequest;
        [ProtoMember( 5)]
        public GetSubscriberRequest GetSubscriberRequest;
        [ProtoMember( 6 )]
        public GetAllSubscribersRequest GetAllSubscribersRequest;
    }
}
