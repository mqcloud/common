using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class ConnectRequest {
        [ProtoMember( 3 )]
        public string NodeUrl;

        [ProtoMember(2)]
        public string BackEndName;

        [ProtoMember(1)]
        public string NodeName;
    }
}