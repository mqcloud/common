using System.Collections.Generic;
using MQCloud.MathModeling.Immitation.Places;

namespace MQCloud.MathModeling.Immitation.Transitions {
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