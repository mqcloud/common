using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class GetSubscriberResponse {
        [ProtoMember(2)]
        public string Node;

        [ProtoMember(1)]
        public int RequestId;
    }
}