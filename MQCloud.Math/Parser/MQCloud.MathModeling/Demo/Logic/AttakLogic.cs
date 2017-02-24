using MQCloud.MathModeling.Demo.Infrastructure;
using MQCloud.MathModeling.Demo.Patterns;
using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Runtime;
using MQCloud.MathModeling.Immitation.Transitions;

namespace MQCloud.MathModeling.Demo.Logic {

    public class InfrastructureLogic {
        
    }

    public class AttakLogic: Immitation.Logic.Logic {
        public LogicHost LogicHost = new LogicHost();
        public WorldHost WorldHost = new WorldHost();
        public RequestResponse Connection;

        private Transition Expect(Place target, int cpu = 0, int network = 0, string name = "expect") {
            return
                Simulation.CreateTransition(name)
                    .AddInOut(WorldHost.CPU)
                    .AddInOut(WorldHost.Network)
                    .SetRequiered<Load>(cpu)
                    .SetRequiered<Kbps>(network)
                    .AddOut(target) as Transition;
        }

        private Transition GetTokens<T>(Place target, int cpu = 0, int network = 0, int count = 1,
            string name = nameof(T)) where T : new() {
            return
                Expect(target, cpu, network, name)
                    .SetOnProcess(place => place.Tokens.AddRange(Simulation.CreateTokens<T>(count))) as Transition;
        }

        public void ApproachTarget() {
            //TODO
            Scedule("Approach", GetTokens<RequestPath>(LogicHost.NPCService, 1, 1), 
                Connection.Request,
                Expect(WorldHost.NavigationService, 1),
                Connection.Acquire,
                GetTokens<ResponsePath>(WorldHost.NavigationService, 1, 1), 
                Connection.Response,
                Expect(LogicHost.NPCService, 1),
                Connection.Consume
                );
        }

        public void AttakTarget() {
            Scedule("Attak",
                GetTokens<RequestRayTrace>(LogicHost.NPCService, 1, 1),
                Connection.Request,
                Expect(WorldHost.CollisionService, 1),
                Connection.Acquire,
                GetTokens<ResponseRayTrace>(WorldHost.CollisionService, 1, 1),
                Connection.Response,
                Expect(LogicHost.NPCService, 1),
                Connection.Consume
                );
        }

        public AttakLogic() {
            Connection = new RequestResponse() {
                Client = LogicHost.NPCService,
                Server = WorldHost.NavigationService
            };
            //token nesting underutilized
            //Pub\Sub

            //Bad network
            //Router vs P2P

            //todo OrderChecker on transition calls!!!
            //Requirements
            //Fix Order
            //Build insertions
            // build connections graph
            // run
            //TODO complex Service logic sample
            //Todo P2P+heartbeat vs Broker
            //Todo TCP+Loss sample (intermidiate\template)
            //Todo Place TAG in simulation to add affector transition

            //Todo HPC arrays connections +data distribiution
            //Todo Mixed HPC+Service
        }
    }
}