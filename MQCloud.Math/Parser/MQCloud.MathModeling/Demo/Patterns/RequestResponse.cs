using MQCloud.MathModeling.Immitation.Patterns;
using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Runtime;
using MQCloud.MathModeling.Immitation.Transitions;

namespace MQCloud.MathModeling.Demo.Patterns {
    public class RequestResponse: Pattern {

        public Place Client;
        public Place Server;

        public Transition Request = Simulation.CreateTransition(nameof(Request))
            .SetRequiered<Request>()
            .SetOrder(1) as Transition;

        public Transition Acquire = Simulation.CreateTransition(nameof(Acquire))
            .SetRequiered<Request>()
            .SetOrder(2)
            .SetOnReturn(place => Token.Empty) as Transition;

        public Transition Response = Simulation.CreateTransition(nameof(Response))
            .SetRequiered<Response>()
            .SetOrder(3)
            .SetOnReturn(place => place.Tokens) as Transition;

        public Transition Consume = Simulation.CreateTransition(nameof(Consume))
            .SetRequiered<Response>()
            .SetOrder(4)
            .SetOnReturn(place => Token.Empty ) as Transition;

        public RequestResponse() {
            Register(
                Request.AddIn(Client).AddOut(Server),
                Acquire.AddIn(Server),
                Response.AddIn(Server).AddOut(Client),
                Consume.AddIn(Client)
            );
        }
    }
}