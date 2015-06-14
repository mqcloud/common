using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public class IncomingEvent {
        [ProtoMember( 1 )]
        public IncomingEventType TypeCode;

        [ProtoMember( 2 )]
        public OnConnectionClosed OnConnectionClosed;
        [ProtoMember( 3 )]
        public OnConnectionEstablished OnConnectionEstablished;
        [ProtoMember( 4 )]
        public OnNodeAdvertisedTopic OnNodeAdvertisedTopic;
        [ProtoMember( 5 )]
        public OnNodeRejectedTopic OnNodeRejectedTopic;
        [ProtoMember( 6 )]
        public OnNodeSubscribedToTopic OnNodeSubscribedToTopic;
        [ProtoMember( 7 )]
        public OnNodeUnavaliable OnNodeUnavaliable;
        [ProtoMember( 8 )]
        public OnNodeUnsubscribedFromTopic OnNodeUnsubscribedFromTopic;
    }
}