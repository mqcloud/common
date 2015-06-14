using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public class RejectTopic {
        [ProtoMember( 1 )]
        public string Pattern;

        [ProtoMember( 2 )]
        public string Topic;
    }
}