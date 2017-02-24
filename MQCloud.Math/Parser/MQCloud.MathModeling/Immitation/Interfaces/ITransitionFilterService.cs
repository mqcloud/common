using System.Collections.Generic;
using MQCloud.MathModeling.Immitation.Transitions;

namespace MQCloud.MathModeling.Immitation.Interfaces {
    public interface ITransitionFilterService {
        ITransitionFilterService Add<T>(int count =1, Mode contained = Mode.Accuire);
        List<TransitionItem> Get();
    }
}