using System;
using System.Collections.Generic;
using System.Linq;

namespace MQCloud.MathModeling {
    public static class Extensions {
        public static Place SetTokens<T>(this Place p, int count) where T : Token, new() {
            p.Tokens.AddRange(Enumerable.Repeat<T>(new T() {
                Type = typeof(T)
            }, count));
            return p;
        }

        public static IEnumerable<T> ForEach<T>(this IEnumerable<T> collection, Action<T> action) {
            foreach (var item in collection) {
                action(item);
            }
            return collection;
        }

        public static Type GetInstance<T>(this Type genericArgument) {
            string elementTypeName = Console.ReadLine();
            Type elementType = Type.GetType(elementTypeName);
            Type[] types = new Type[] { elementType };

            Type listType = typeof(List<>);
            Type genericType = listType.MakeGenericType(types);
            T result = (T)Activator.CreateInstance(genericType);
            return result;
        }
    }
}