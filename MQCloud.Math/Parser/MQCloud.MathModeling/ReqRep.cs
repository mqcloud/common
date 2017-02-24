using System.Collections.ObjectModel;
using MQCloud.MathModeling.Immitation.Patterns;
using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Runtime;
using Transition = MQCloud.MathModeling.Immitation.Transitions.Transition;

namespace MQCloud.MathModeling {
    public class Subscribe<T>: Pattern {
        public Places Clients;
        public Place Server;

        public Transition Subsribe =
            Simulation.CreateTransition(nameof(Subsribe)).SetRequiered<T>().SetOrder(1) as Transition;

        public Transition Acquire =
            Simulation.CreateTransition(nameof(Acquire)).SetRequiered<T>().SetOrder(2) as Transition;

        public Subscribe() {
            Order.OrderForEach(Clients);
            Clients.SetStencil(ctx => {
                var current = ctx.GetCurrent();
                Subsribe.AddIn(current).AddOut(Server);
            });
            Acquire.AddInOut(Server);
            Register(Subsribe, Acquire);
        }
    }

    //Todo eraize items from simulation when presented with absentElementConstructed places!
    public class PublishSubscribe<TSubscribe, TEvent>: Pattern {
        public Places Clients {
            get { return Subscribe.Clients; }
            set { Subscribe.Clients = value; }
        }

        public Place Server {
            get { return Subscribe.Server; }
            set { Subscribe.Server = value; }
        }

        public Subscribe<TSubscribe> Subscribe = new Subscribe<TSubscribe>(); 

        public Transition Publish =
            Simulation.CreateTransition(nameof(Publish))
                .SetRequiered<TEvent>()
                .SetOrder(1)
                .SetOnReturn(place => place.Tokens) as Transition;

        public Transition Consume =
            Simulation.CreateTransition(nameof(Consume))
                .SetRequiered<TEvent>()
                .SetOrder(2)
                .SetOnReturn(place => Token.Empty) as Transition;

        public PublishSubscribe() {
            Order.OrderForEach(Clients);
            Order.Require(Subscribe, 2);
            Clients.SetStencil(ctx => {
                var current = ctx.GetCurrent();
                Publish.AddIn(Server).AddOut(current);
            });
            Register(Publish, Consume);
        }
    }


    /* //todo
        public class BadNetwork : Transition {
            Place BadRouter;
            Place Source;
            Place Target;

            public BadNetwork(Place @from, Place to) : base(@from, to) {}
        }

    */


}
