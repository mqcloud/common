using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public enum OutgoingEventType {
        AdvertizeTopic,
        Disconnect,
        HeartBeat,
        RejectTopic,
        ReportNodeUnavaliable,
        Subscribe,
        UnSubscribe
    }
}