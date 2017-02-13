using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace MQCloud.MathModeling {
    public class Logic {
        //Keep the Scedule order in check
        private int _id = 0;

        //Keep places order used in logic
        private readonly List<Place> _sceduleAlgorithm = new List<Place>();

        //Keep temporary transitions used in logic
        private readonly List<ITransition> _sceduleTransitions = new List<ITransition>();

        protected void Scedule(string groupName = "", params ITransitionService[] transitionService) {
            transitionService.ForEach(service => Scedule(service, groupName));
        }

        //Surround add transition with p(s)->t->p(s) to controll logic flow
        protected void Scedule(ITransitionService transitionService, [CallerMemberName] string memberName = "") {
            var transition = transitionService.GetTransition();
            _sceduleTransitions.Add(transition);
            var T = this.GetType().GetGenericInstanceOf<Scedule>().GetType(); //Todo: UnitTest: T shall be typeof(Scedule<thisChildT>);

            transition.Connections.Where(subscriber => subscriber.ConnectionDirection == Direction.Out &&
                                                       subscriber.Filter.All(item => item.TokenType != T))
                .ForEach(subscriber => subscriber.Filter.AddRange(RequirementsBuilder
                    .Create()
                    .Add(T, 1, Mode.Ignore)
                    .Get()));
            
            if (transition.Requirements.All(item => item.TokenType != typeof (Scedule))) {
                transition.Requirements.AddRange(RequirementsBuilder.Get(T, 1));
            }

            Place before = null;
            if (_id - 1 == 0) {
                before = Simulation.GetPlace(memberName + ++_id);
                _sceduleAlgorithm.Add(before);
                before.Add(T);
            }

            Place after = Simulation.GetPlace(memberName + ++_id);
            _sceduleAlgorithm.Add(after);

            transitionService.AddIn(before);
            transitionService.AddOut(after, RequirementsBuilder.Create()
                .Add(T).Get()
                );
        }

        //Pack an action into transition OnReturn and insert it into Scedule chain
        protected void Scedule(Action action, [CallerMemberName] string memberName = "") {
            Transition t = Simulation.CreateTransition(memberName + _id);
            t.SetOnReturn(place => {
                action();
                return place.Tokens;
            });
            Scedule(t, memberName);
        }
    }
}