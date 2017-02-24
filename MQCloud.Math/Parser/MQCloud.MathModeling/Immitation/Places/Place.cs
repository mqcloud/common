using System;
using System.Collections.Generic;
using System.Linq;
using MQCloud.MathModeling.Immitation.Interfaces;
using MQCloud.MathModeling.Immitation.Transitions;

namespace MQCloud.MathModeling.Immitation.Places {
    public class Place : ITaggable<Place>{
        public readonly List<Token> TokensAdded = new List<Token>();
        public readonly List<Token> TokensRemoved = new List<Token>();
        public readonly List<Token> Tokens = new List<Token>();
        public readonly List<ITransition> ConnectedTransitions = new List<ITransition>();
        public readonly HashSet<Tag> Tags = new HashSet<Tag>();
        public Scope Scope;


        public string Id;
        public List<TransitionItem> Filter { get; set; }

        public Place SetTag(Tag tag) {
            Tags.Add(tag);
            return this;
        }

        public Place RemoveTag(Tag tag) {
            Tags.Remove(tag);
            return this;
        }

        public List<string> GetTags() {
            return Tags.Select(tag => tag.Name).ToList();
        }

        public bool Has<T>(int i) {
            return Tokens.Count(token => token.Type == typeof(T)) == i;
        }

        public void Add(Type T, int count = 1) {
            Tokens.AddRange(Runtime.Simulation.CreateTokens(T, count));
        }

        public void Add<T>(int count = 1) where T : new() {
            Tokens.AddRange(Runtime.Simulation.CreateTokens<T>(count));
        }

    }
}