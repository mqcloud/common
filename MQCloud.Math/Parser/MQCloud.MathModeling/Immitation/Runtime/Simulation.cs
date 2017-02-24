using System;
using System.Collections.Generic;
using System.Linq;
using MQCloud.MathModeling.Immitation.Places;
using MQCloud.MathModeling.Immitation.Transitions;
using MQCloud.MathModeling.Immitation.Utils;

namespace MQCloud.MathModeling.Immitation.Runtime {
    public static class Simulation {
        private static readonly Factory<Token> TokenFactory = new Factory<Token>();
        private static readonly Factory<Place> PlaceFactory = new Factory<Place>();
        private static readonly Factory<Transition> TransitionFactory = new Factory<Transition>();

        private static void Cleanup() {
            throw new NotImplementedException();
        }

        public static void Run() {
            throw new NotImplementedException();
        }

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
        public static Place CreatePlace(string id) {
            return PlaceFactory.Generate<Place>();
        }
    }
}