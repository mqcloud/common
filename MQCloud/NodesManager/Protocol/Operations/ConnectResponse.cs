using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class ConnectResponse {
        [ProtoMember( 1 )]
        public string RealNodeName;
        [ProtoMember( 2 )]
        public int HeartBeatRateMs;
    }
}