namespace NodesManager.Protocol {
    //General
    public class ConnectRequest {
        public string NodeName;
    }

    public class ConnectResponse {
        public string RealNodeName;
    }

    public class HeartBeat {}

    public class Disconnect {}

    //Out
    public class AdvertizeTopic {
        public string Pattern;
        public string Topic;
    }

    public class GetAllPublishersRequest {
        public string BackEndName;
        public string Pattern;
        public int RequestId;
        public string Topic;
    }

    public class GetAllPublishersResponse {
        public string[] Nodes;
        public int RequestId;
    }

    public class GetPublisherRequest {
        public string BackEndName;
        public string Pattern;
        public int RequestId;
        public string Topic;
    }

    public class GetPublishersResponse {
        public string[] Nodes;
        public int RequestId;
    }

    public class RejectTopic {
        public string Pattern;
        public string Topic;
    }

    //In
    public class Subscribe {
        public string Pattern;
        public string Topic;
    }

    public class GetAllSubscribersRequest {
        public string BackEndName;
        public string Pattern;
        public int RequestId;
        public string Topic;
    }

    public class GetAllSubscribersResponse {
        public string[] Nodes;
        public int RequestId;
    }

    public class GetSubscriberRequest {
        public string BackEndName;
        public string Pattern;
        public int RequestId;
        public string Topic;
    }

    public class GetSubscriberResponse {
        public string Node;
        public int RequestId;
    }

    public class UnSubscribe {
        public string Pattern;
        public string Topic;
    }

    //events
    public class OnAdvertisedTopic {
        public string Node;
        public string Pattern;
        public string Topic;
    }

    public class OnNodeRejectedTopic {
        public string Node;
        public string Pattern;
        public string Topic;
    }
}