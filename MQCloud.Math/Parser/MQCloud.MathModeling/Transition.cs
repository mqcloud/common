using System;
using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public class Transition : ITransition, ITransitionService {
        private static int idCounter = 0;
        private int order = 0;
        private bool isInitialized = false;

        internal Transition SetName(string name) {
            if (!isInitialized) {
                Container = Simulation.GetPlace(string.Format("{0}_{1}", name, idCounter++));
                isInitialized = true;
            }
            return this;
        }

        private Action<Place> _process;
        private Func<Place, List<Token>> _result = place => place.Tokens;
        private bool _once = false;
        private OrderChecker _orderChecker;

        public List<TransitionItem> Requirements { get; set; }
        public List<TransitionSubscriber> Connections { get; }
        public Place Container { get; set; }

        public virtual void Process() {
            _process?.Invoke(Container);
        }

        public List<Token> Return() {
            return _result?.Invoke(Container);
        }

        public ITransitionService SetOnProcess(Action<Place> process) {
            _process = process;
            return this;
        }

        public ITransitionService SetOnReturn(Func<Place, List<Token>> result) {
            _result = result;
            return this;
        }

        public ITransitionService SetRequirements(List<TransitionItem> requirements) {
            Requirements = requirements;
            return this;
        }

        public ITransitionService SetRequiered<T>( int count = 1) {
            Requirements.AddRange(RequirementsBuilder.Get<T>(count));
            return this;
        }

        public ITransitionService SetOnce() {
            _once = true;
            return this;
        }

        public ITransitionService SetOrder(int callOrder) {
            order = callOrder;
            return this;
        }

        public ITransitionService AddIn(Place @from) {
            return AddIn(@from, null);
        }

        public ITransitionService AddIn(Place @from, List<TransitionItem> affectOnly) {
            if (affectOnly == null) {
                affectOnly = new List<TransitionItem>();
            }
            affectOnly.AddRange(@from.Filter);
            Connections.Add(new TransitionSubscriber() {
                Subscriber = @from,
                ConnectionDirection = Direction.In,
                Filter = affectOnly
            });
            return this;
        }

        public ITransitionService AddOut(Place to) {
            return AddOut(to, null);
        }

        public ITransitionService AddOut(Place to, List<TransitionItem> putOnly) {
            Connections.Add(new TransitionSubscriber() {
                Subscriber = to,
                ConnectionDirection = Direction.Out,
                Filter = putOnly
            });
            return this;
        }

        public ITransitionService AddOrderChecker(OrderChecker checker) {
            _orderChecker = checker;
            return this;
        }

        public ITransitionService  AddInOut(Place place) {
            AddIn(place).AddOut(place);
            return this;
        }

        public ITransition GetTransition() {
            return this;
        }
        
    }
}