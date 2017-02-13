using System;
using System.Collections.Generic;
using System.Linq;

namespace MQCloud.MathModeling {
    public static class Extensions {
        //todo move into place interface
        // Put tokens of type T into this place
        public static Place SetTokens<T>(this Place p, int count) where T : Token, new() {
            p.Tokens.AddRange(Enumerable.Repeat<T>(new T() {
                Type = typeof(T)
            }, count));
            return p;
        }

        // For each element in the collection do the action
        public static IEnumerable<T> ForEach<T>(this IEnumerable<T> collection, Action<T> action) {
            foreach (var item in collection) {
                action(item);
            }
            return collection;
        }

        // Create an instance of type T<this>
        public static T GetGenericInstanceOf<T>(this Type genericArgument) {
            var baseType = typeof(T);
            var genericType = baseType.MakeGenericType(genericArgument);
            var result = (T)Activator.CreateInstance(genericType);
            return result;
        }
        
        public static T GetInstance<T>(this Type type) {
            var result = (T)Activator.CreateInstance(type);
            return result;
        }
    }
}