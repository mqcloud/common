using MQCloud.MathModeling.Immitation.Interfaces;
using MQCloud.MathModeling.Immitation.Utils;

namespace MQCloud.MathModeling.Immitation.Patterns {
    public class Pattern {
        protected OrderChecker Order = OrderChecker.Get();

        protected void Register(params ITransitionService[] t) {
            t.ForEach(service => service.AddOrderChecker(Order));
        }
    }
}