using System.Collections.Generic;
using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Transitions;

namespace MQCloud.MathModeling.Immitation.Interfaces {
    public interface ITransition {
        List<TransitionItem> Requirements { get; }
        List<TransitionSubscriber> Connections { get; }

        Place Container { get; }

        void Process();

        List<Token> Return();
    }
}