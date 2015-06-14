using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class GetPublisherResponse {
        [ProtoMember( 2 )]
        public string Node;

        [ProtoMember( 1 )]
        public int RequestId;
    }
}