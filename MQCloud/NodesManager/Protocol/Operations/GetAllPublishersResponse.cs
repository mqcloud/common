using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class GetAllPublishersResponse {
        [ProtoMember(2)]
        public string[] Nodes;

        [ProtoMember(1)]
        public int RequestId;
    }
}