using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class ConnectResponse {
        [ProtoMember(2)]
        public int HeartBeatRateMs;

        [ProtoMember(1)]
        public string RealNodeName;
    }
}