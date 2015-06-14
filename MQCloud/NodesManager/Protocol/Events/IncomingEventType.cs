using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public enum IncomingEventType {
        OnConnectionClosed,
        OnConnectionEstablished,
        OnNodeAdvertisedTopic,
        OnNodeRejectedTopic,
        OnNodeSubscribedToTopic,
        OnNodeUnavaliable,
        OnNodeUnsubscribedFromTopic
    }
}