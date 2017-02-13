using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Dynamic;
using System.Linq;
using Graphviz4Net.Graphs;
using Irony.Parsing;
using Irony.Parsing.Construction;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using MQCloud.MathModeling;

namespace MQCloud.MathModeling {
 
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
        private int _defaultValue;
        private int _totallStates;

        public static OrderChecker Get(int startState = 0) {
            return new OrderChecker(startState);
        }

        OrderChecker(int startState) {
            _defaultValue = startState;
        }

        public OrderChecker SetCount(int totallStates) {
            _totallStates = totallStates;
            return this;
        }
        
        private readonly Dictionary<Place, int> PerPlaceState = new Dictionary<Place, int>(); 
        public bool CheckAndSet(ITransition caller, int newState, bool once) {
            //TODO ForEach 
            bool result;
            var currentState = 1;// TODO here

            if(newState < currentState) {
                if(newState == 1 && currentState == _totallStates) {
                    currentState = newState;
                    result = true;
                } else {
                    result = false;
                }
            } else {
                result = !once
                    ? (newState > currentState ? newState == ++currentState : newState == currentState)
                    : newState == ++currentState;

                if(!result) {
                    currentState--;
                }
            }

            return result;
        }

        public void OrderForEach(ObservableCollection<Place> clients) {
            clients.CollectionChanged += (sender, args) => {
                switch (args.Action) {
                    case NotifyCollectionChangedAction.Add:
                        foreach (var newItem in args.NewItems.Cast<Place>()) {
                            if (newItem != null && !PerPlaceState.ContainsKey(newItem)) {
                                PerPlaceState.Add(newItem, 0);
                            }
                        }
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();

                }
            };
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

            protected void Register(params ITransitionService[] t) {
                t.ForEach(service => service.AddOrderChecker(Order));
            }
        }

        public class Subscribe : Pattern {
            public Place[] Clients;
            public Place Server;

            public Transition Subsribe = Simulation.CreateTransition(nameof(Subsribe)).SetRequiered<Topik>().SetOrder(1) as Transition;

            public Transition Acquire = Simulation.CreateTransition(nameof(Acquire)).SetRequiered<Topik>().SetOrder(2) as Transition;

            public Subscribe() {
                OrderChecker.OrderForEach(Clients);

                Register(Subsribe, Acquire);
            }
        }

        public class PublishSubscribe : Pattern {
            public Transition Response = Simulation.CreateTransition(nameof(Response)).SetRequiered<Response>().SetOrder(1).SetOnReturn(place => place.Tokens) as Transition;

            public Transition Consume = Simulation.CreateTransition(nameof(Consume)).SetRequiered<Response>().SetOrder(2).SetOnReturn(place => place.Tokens) as Transition;

            public PublishSubscribe() {}
        }

        public class Topik : Token {}

        public class AttakLogic : Logic {
            public LogicHost LogicHost = new LogicHost();
            public WorldHost WorldHost = new WorldHost();
            public RequestResponse Connection;

            private Transition Expect(Place target, int cpu = 0, int network = 0, string name = "expect") {
                return Simulation.CreateTransition(name).AddInOut(WorldHost.CPU).AddInOut(WorldHost.Network).SetRequiered<Load>(cpu).SetRequiered<Kbps>(network).AddOut(target) as Transition;
            }

            private Transition GetTokens<T>(Place target, int cpu = 0, int network = 0, int count = 1, string name = nameof(T)) where T : new() {
                return Expect(target, cpu, network, name).SetOnProcess(place => place.Tokens.AddRange(Simulation.CreateTokens<T>(count))) as Transition;
            }

            public void ApproachTarget() {
                //TODO
                Scedule("Approach", GetTokens<RequestPath>(LogicHost.NPCService, 1, 1), Connection.Request, Expect(WorldHost.NavigationService, 1), Connection.Acquire, GetTokens<ResponsePath>(WorldHost.NavigationService, 1, 1), Connection.Response, Expect(LogicHost.NPCService, 1), Connection.Consume);
            }

            public void AttakTarget() {
                Scedule("Attak", GetTokens<RequestRayTrace>(LogicHost.NPCService, 1, 1), Connection.Request, Expect(WorldHost.CollisionService, 1), Connection.Acquire, GetTokens<ResponseRayTrace>(WorldHost.CollisionService, 1, 1), Connection.Response, Expect(LogicHost.NPCService, 1), Connection.Consume);
            }

            AttakLogic() {
                Connection = new RequestResponse() {
                    Client = LogicHost.NPCService, Server = WorldHost.NavigationService
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

        public class ResponseRayTrace : Token {}

        public class RequestRayTrace : Token {}

        internal class ResponsePath : Request {}

        internal class RequestPath : Request {}

        public class BadNetwork : Transition {
            Place BadRouter;
            Place Source;
            Place Target;

            public BadNetwork(Place @from, Place to) : base(@from, to) {}
        }

        public class Token {
            public Type Type;

            Token paret;
            List<Token> children;
            public Token() {}
        }

        public class Id : Token {}

        public class Message : Token {}

        public class Request : Token {}

        public class Response : Token {}

        public class Kbps : Token {}

        public class Load : Token {}

        public class Scedule : Token {}

        public class Scedule<T> : Scedule {}
    }