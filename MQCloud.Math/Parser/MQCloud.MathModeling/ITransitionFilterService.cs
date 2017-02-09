using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public interface ITransitionFilterService {
        ITransitionFilterService Add<T>(int count =1, Mode contained = Mode.Accuire);
        List<TransitionItem> Get();
    }
}