﻿using ProtoBuf;

namespace NodesManager.Protocol.Operations {
    [ProtoContract]
    public class GetSubscriberRequest {
        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(1)]
        public int RequestId;

        [ProtoMember(3)]
        public string Topic;
    }
}