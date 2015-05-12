using ProtoBuf;

namespace NodesManager.Protocol {
    //General
    [ProtoContract]
    public class ConnectRequest {
        [ProtoMember(1)]
        public string NodeName;
    }

    [ProtoContract]
    public class ConnectResponse {
        [ProtoMember(1)]
        public string RealNodeName;
    }

    [ProtoContract]
    public class HeartBeat {}

    [ProtoContract]
    public class Disconnect {}

    //Out
    [ProtoContract]
    public class AdvertizeTopic {
        [ProtoMember(1)]
        public string Pattern;

        [ProtoMember(2)]
        public string Topic;
    }

    [ProtoContract]
    public class GetAllPublishersRequest {
        [ProtoMember(3)]
        public string BackEndName;

        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(1)]
        public int RequestId;

        [ProtoMember(4)]
        public string Topic;
    }

    [ProtoContract]
    public class GetAllPublishersResponse {
        [ProtoMember(2)]
        public string[] Nodes;

        [ProtoMember(1)]
        public int RequestId;
    }

    [ProtoContract]
    public class GetPublisherRequest {
        [ProtoMember(3)]
        public string BackEndName;

        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(1)]
        public int RequestId;

        [ProtoMember(4)]
        public string Topic;
    }

    [ProtoContract]
    public class GetPublishersResponse {
        [ProtoMember(2)]
        public string[] Nodes;

        [ProtoMember(1)]
        public int RequestId;
    }

    [ProtoContract]
    public class RejectTopic {
        [ProtoMember(1)]
        public string Pattern;

        [ProtoMember(2)]
        public string Topic;
    }

    //In
    [ProtoContract]
    public class Subscribe {
        [ProtoMember(1)]
        public string Pattern;

        [ProtoMember(2)]
        public string Topic;
    }

    [ProtoContract]
    public class GetAllSubscribersRequest {
        [ProtoMember(3)]
        public string BackEndName;

        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(1)]
        public int RequestId;

        [ProtoMember(4)]
        public string Topic;
    }

    [ProtoContract]
    public class GetAllSubscribersResponse {
        [ProtoMember(2)]
        public string[] Nodes;

        [ProtoMember(1)]
        public int RequestId;
    }

    [ProtoContract]
    public class GetSubscriberRequest {
        [ProtoMember(3)]
        public string BackEndName;

        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(1)]
        public int RequestId;

        [ProtoMember(4)]
        public string Topic;
    }

    [ProtoContract]
    public class GetSubscriberResponse {
        [ProtoMember(2)]
        public string Node;

        [ProtoMember(1)]
        public int RequestId;
    }

    [ProtoContract]
    public class UnSubscribe {
        [ProtoMember(1)]
        public string Pattern;

        [ProtoMember(2)]
        public string Topic;
    }

    //events
    [ProtoContract]
    public class OnAdvertisedTopic {
        [ProtoMember(1)]
        public string Node;

        [ProtoMember(2)]
        public string Pattern;

        [ProtoMember(3)]
        public string Topic;
    }

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