using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public class OnConnectionEstablished {
        [ProtoMember( 1 )]
        public string FromNode;
        [ProtoMember( 2 )]
        public string ToNode;
    }
}