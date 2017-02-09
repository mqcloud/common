using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace MQCloud.MathModeling {
    public class Logic {
        private static int _id = 0;
        private readonly List<Place> _sceduleAlgorithm = new List<Place>();

        protected void Scedule(Transition transition, [CallerMemberName] string memberName = "") {
            var T = this.GetType().GetInstance<Scedule>();



            transition.Connections.Where(subscriber => subscriber.ConnectionDirection == Direction.Out &&
                                                       subscriber.Filter.All(item => item.TokenType != T))
                .ForEach(subscriber => subscriber.Filter.AddRange(RequirementsBuilder
                    .Create()
                    .Add(T, 1, Mode.Ignore)
                    .Get()));

            if (transition.Requirements.All(item => item.TokenType != typeof (Scedule))) {
                transition.Requirements.AddRange(RequirementsBuilder.Create().Add(T, 1).Get());
            }

            Place before = null;
            if (_id - 1 == 0) {
                before = Simulation.GetPlace(memberName + ++_id);
                _sceduleAlgorithm.Add(before);
                before.Add(T);
            }

            Place after = Simulation.GetPlace(memberName + ++_id);
            _sceduleAlgorithm.Add(after);

            transition.AddIn(before);
            transition.AddOut(after, RequirementsBuilder.Create()
                .Add(T).Get()
                );
        }

        protected void Scedule(Action action, [CallerMemberName] string memberName = "") {
            Transition t = Simulation.CreateTransition(memberName + _id);
            t.SetOnReturn(place => {
                action();
                return place.Tokens;
            });
            Scedule(t);
        }
    }
}