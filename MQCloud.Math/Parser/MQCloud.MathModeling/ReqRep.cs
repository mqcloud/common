using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Dynamic;
using System.Linq;
using Graphviz4Net.Graphs;
using Irony.Parsing;
using Irony.Parsing.Construction;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using MQCloud.MathModeling;

namespace MQCloud.MathModeling {
    public class Token {
        public Type Type;

        Token paret;
        List<Token> children;
        public Token() { }
    }

    public static class Simulation {
        private static readonly Factory<Token> TokenFactory = new Factory<Token>();
        private static readonly Factory<Place> PlaceFactory = new Factory<Place>();
        private static readonly Factory<Transition> TransitionFactory = new Factory<Transition>();

        public static List<Token> CreateTokens<T>(int count = 1) where T : new() {
            var result = Enumerable.Range(0, count).Select(i => TokenFactory.Generate<T>()).ToList();
            return result;
        }

        public static List<Token> CreateTokens(Type T, int count = 1) {
            var result = Enumerable.Range(0, count).Select(i => TokenFactory.Generate(T)).ToList();
            return result;
        }

        public static Token GetToken(string id) {
            return TokenFactory.Get(id);
        }
        public static Place GetPlace(string id) {
            return PlaceFactory.Get(id);
        }
        public static Transition GetTransition(string id) {
            return TransitionFactory.Get(id).SetName(id);
        }
        public static Transition CreateTransition(string id) {
            return TransitionFactory.Generate<Transition>().SetName(id);
        }
    }

    public class Place {
        public readonly List<Token> TokensAdded = new List<Token>();
        public readonly List<Token> TokensRemoved = new List<Token>();
        public readonly List<Token> Tokens = new List<Token>();
        public readonly List<ITransition> ConnectedTransitions = new List<ITransition>();

        public string Id;
        public List<TransitionItem> Filter { get; set; }



        public bool Has<T>(int i) {
            return Tokens.Count(token => token.Type == typeof(T)) == i;
        }

        public void Add(Type T, int count = 1) {
            Tokens.AddRange(Simulation.CreateTokens(T, count));
        }

        public void Add<T>(int count = 1) where T : new() {
            Tokens.AddRange(Simulation.CreateTokens<T>(count));
        }

    }

    public class OrderChecker {
        private int _currentState;
        private int _totallStates;

        public static OrderChecker Get(int startState = 0) {
            return new OrderChecker(startState);
        }

        OrderChecker(int startState) {
            _currentState = startState;
        }

        public OrderChecker SetCount(int totallStates) {
            _totallStates = totallStates;
            return this;
        }

        public bool CheckAndSet(int newState, bool once) {
            bool result;
            if(newState < _currentState) {
                if(newState == 1 && _currentState == _totallStates) {
                    _currentState = newState;
                    result = true;
                } else {
                    result = false;
                }
            } else {
                result = !once
                    ? (newState > _currentState ? newState == ++_currentState : newState == _currentState)
                    : newState == ++_currentState;

                if(!result) {
                    _currentState--;
                }
            }

            return result;
        }
    }

    public class WorldHost {
        public Place NavigationService = Simulation.GetPlace("NS");
        public Place CollisionService = Simulation.GetPlace("CS");
        public Place Network = Simulation.GetPlace("WHN").SetTokens<Kbps>(10000);
        public Place CPU = Simulation.GetPlace("WHCPU").SetTokens<Load>(100);
    }

    public class LogicHost {
        public Place NPCService = Simulation.GetPlace(nameof(NPCService));
        public Place GatewayService = Simulation.GetPlace(nameof(GatewayService));
        public Place Network = Simulation.GetPlace("LNW").SetTokens<Kbps>(10000);
        public Place CPU = Simulation.GetPlace("LCPU").SetTokens<Load>(100);
    }


    public class Pattern {
        protected OrderChecker Order = OrderChecker.Get();

        protected void Register(ITransitionService t) {
            t.AddOrderChecker(Order);
        }
    }
    public class RequestResponse: Pattern {

        public Place Client;
        public Place Server;

        public Transition Request = Simulation.CreateTransition(nameof(Request))
            .SetRequiered<Request>()
            .SetOrder(1)
            .SetOnReturn(place => place.Tokens) as Transition;

        public Transition Acquire = Simulation.CreateTransition(nameof(Acquire))
            .SetRequiered<Request>()
            .SetOrder(2)
            .SetOnReturn(place => place.Tokens) as Transition;

        public Transition Response = Simulation.CreateTransition(nameof(Response))
            .SetRequiered<Response>()
            .SetOrder(3)
            .SetOnReturn(place => place.Tokens) as Transition;

        public Transition Consume = Simulation.CreateTransition(nameof(Consume))
            .SetRequiered<Response>()
            .SetOrder(4)
            .SetOnReturn(place => place.Tokens) as Transition;

        public RequestResponse() {
            Request.AddIn(Client).AddOut(Server);
            Register(Request);
            Response.AddIn(Server).AddOut(Client);
            Register(Response);
        }
    }

    public class AttakLogic: Logic {
        public LogicHost LogicHost = new LogicHost();
        public WorldHost WorldHost = new WorldHost();
        public RequestResponse Connection;

        AttakLogic() {
            Connection = new RequestResponse() {
                Client = LogicHost.NPCService,
                Server = WorldHost.NavigationService
            };
            LogicHost.NPCService.SetTokens<RequestPath>(1);
            //todo OrderChecker on transition calls!!!

            //Fix Order
            //Build insertions
            // build connections graph
            // run
            Scedule(() => LogicHost.NPCService.Add<RequestPath>());
            Scedule(Connection.Request);
            Scedule(Connection.Acquire);
            Transition GenerateResponse =
                Simulation.CreateTransition(nameof(GenerateResponse))
                .AddInOut(WorldHost.CPU)
                .AddInOut(WorldHost.Network)
                .SetRequiered<Load>(12)
                .SetRequiered<Kbps>(100)
                    .AddOut(WorldHost.NavigationService)
                    .SetOnProcess(place => place.Tokens.AddRange(Simulation.CreateTokens<ResponsePath>())) as Transition;

            Scedule(GenerateResponse); //Todo keep transition safe!
            Scedule(Connection.Response);
            Scedule(Connection.Consume);

            //TODO complex Service logic sample
            //Todo P2P+heartbeat vs Broker
            //Todo TCP+Loss sample (intermidiate\template)
            //Todo Place TAG in simulation to add affector transition

            //Todo HPC arrays connections +data distribiution
            //Todo Mixed HPC+Service
        }
    }

    internal class ResponsePath: Request { }

    internal class RequestPath: Request { }

    public class BadNetwork: Transition {
        Place BadRouter;
        Place Source;
        Place Target;

        public BadNetwork(Place @from, Place to) : base(@from, to) {

        }
    }

    public class Id: Token { }
    public class Message: Token { }
    public class Request: Token { }
    public class Response: Token { }
    public class Kbps: Token { }
    public class Load: Token { }

    public class Scedule: Token { }
    public class Scedule<T>: Scedule { }

}