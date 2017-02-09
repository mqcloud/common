using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public enum Direction {
        Default = 0,
        In,
        Out
    }
    public class TransitionSubscriber {
        
        public Place Subscriber;
        public Direction ConnectionDirection;
        public List<TransitionItem> Filter;
    }
}