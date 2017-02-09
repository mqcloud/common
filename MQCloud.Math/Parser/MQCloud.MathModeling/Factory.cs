using System;
using System.Collections.Concurrent;
using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public sealed class Factory<T> where T : class, new() {
        public static ConcurrentDictionary<string, T> Arena = new ConcurrentDictionary<string,T>();
        private static int _id = 0;
        public T Generate(Type T2) {
            string elementTypeName = Console.ReadLine();
            Type elementType = Type.GetType(elementTypeName);
            Type[] types = new Type[] { elementType };

            Type listType = typeof(List<>);
            Type genericType = listType.MakeGenericType(types);
            T result = (T) Activator.CreateInstance(genericType);
            var id = (++_id).ToString();
            if(!Arena.TryAdd(id, result)) {
                result = Generate(T2);
            }
            return result;
        }

        public T Generate<T2>() where T2 : new() {
            T result = new T2() as T;
            var id = (++_id).ToString();
            if (!Arena.TryAdd(id, result)) {
                result = Generate<T2>();
            }
            return result;
        }
        public T Get(string id) {
            T result = new T();
            if(!Arena.TryAdd(id, result)) {
                Arena.TryGetValue(id, out result);
            } else {
                ++_id;
            }
            return result;
        }

    }
}