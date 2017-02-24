using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using MQCloud.MathModeling.Immitation.Interfaces;
using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Utils;

namespace MQCloud.MathModeling.Immitation.Patterns {
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
            var currentState = 1; // TODO here

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

        public void OrderForEach(Places.Places clients) {
            clients.Initialize();
            clients.GetRawData().ForEach(p => {
                if (!PerPlaceState.ContainsKey(p)) {
                    PerPlaceState.Add(p, 0);
                }
            });
        }
    }
}