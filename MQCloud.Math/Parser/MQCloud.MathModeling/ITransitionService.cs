using System;
using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public interface ITransitionService {
        ITransitionService SetOnProcess(Action<Place> process);
        ITransitionService SetOnReturn(Func<Place, List<Token>> result);
        ITransitionService SetRequirements(List<TransitionItem> requirements);
        ITransitionService SetRequiered<T>(int count = 1);
        ITransitionService SetOnce();
        ITransitionService SetOrder(int callOrder);

        ITransitionService AddIn(Place from);
        ITransitionService AddIn(Place from, List<TransitionItem> affectOnly);

        ITransitionService AddOut(Place to);
        ITransitionService AddOut(Place to, List<TransitionItem> putOnly);

        ITransitionService AddOrderChecker(OrderChecker check);

        ITransitionService AddInOut(Place place);
    }
}