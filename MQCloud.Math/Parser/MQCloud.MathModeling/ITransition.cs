using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public interface ITransition {
        List<TransitionItem> Requirements { get; }
        List<TransitionSubscriber> Connections { get; }

        Place Container { get; }

        void Process();

        List<Token> Return();
    }
}