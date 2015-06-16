using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public class OutgoingEvent {
        [ProtoMember(2)]
        public AdvertizeTopic AdvertizeTopic;

        [ProtoMember(3)]
        public Disconnect Disconnect;

        [ProtoMember(4)]
        public RejectTopic RejectTopic;

        [ProtoMember(5)]
        public ReportNodeUnavaliable ReportNodeUnavaliable;

        [ProtoMember(6)]
        public Subscribe Subscribe;

        [ProtoMember(1)]
        public OutgoingEventType TypeCode;

        [ProtoMember(7)]
        public UnSubscribe UnSubscribe;
    }
}