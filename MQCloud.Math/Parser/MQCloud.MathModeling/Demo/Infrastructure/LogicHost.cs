using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Runtime;
using MQCloud.MathModeling.Immitation.Utils;

namespace MQCloud.MathModeling.Demo.Infrastructure {
    public class LogicHost {
        public Place NPCService = Simulation.GetPlace(nameof(NPCService));
        public Place GatewayService = Simulation.GetPlace(nameof(GatewayService));
        public Place Network = Simulation.GetPlace("LNW").SetTokens<Kbps>(10000);
        public Place CPU = Simulation.GetPlace("LCPU").SetTokens<Load>(100);
    }
}