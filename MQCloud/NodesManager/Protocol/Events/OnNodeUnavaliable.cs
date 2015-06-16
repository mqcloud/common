using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public class OnNodeUnavaliable {
        [ProtoMember(1)]
        public string Node;
    }
}