﻿using ProtoBuf;

namespace NodesManager.Protocol.Events {
    [ProtoContract]
    public class OnNodeRejectedTopic {
        [ProtoMember(1)]
        public string Node;

        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(3)]
        public string Topic;
    }
}