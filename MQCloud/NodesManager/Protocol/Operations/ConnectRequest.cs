using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class ConnectRequest {
        [ProtoMember( 1 )]
        public string NodeName;
        [ProtoMember( 2 )]
        public string BackEndName;
    }
}