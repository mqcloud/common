using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class IncomingOperation {
        [ProtoMember( 1 )]
        public IncomingOperationType TypeCode;

        [ProtoMember( 2 )]
        public ConnectResponse ConnectResponse;
        [ProtoMember( 3 )]
        public GetPublisherResponse GetPublisherResponse;
        [ProtoMember( 4 )]
        public GetAllPublishersResponse GetAllPublishersResponse;
        [ProtoMember( 5 )]
        public GetSubscriberResponse GetSubscriberResponse;
        [ProtoMember( 6 )]
        public GetAllSubscribersResponse GetAllSubscribersResponse;
    }
}