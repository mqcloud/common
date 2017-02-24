using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Runtime;
using MQCloud.MathModeling.Immitation.Utils;

namespace MQCloud.MathModeling.Demo.Infrastructure {
    public class WorldHost {
        public Place NavigationService = Simulation.GetPlace("NS");
        public Place CollisionService = Simulation.GetPlace("CS");
        public Place Network = Simulation.GetPlace("WHN").SetTokens<Kbps>(10000);
        public Place CPU = Simulation.GetPlace("WHCPU").SetTokens<Load>(100);
    }
}