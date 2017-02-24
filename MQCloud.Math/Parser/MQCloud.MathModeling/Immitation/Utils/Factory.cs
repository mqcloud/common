using System;
using System.Collections.Concurrent;
using System.Runtime.InteropServices;

namespace MQCloud.MathModeling.Immitation.Utils {
    public sealed class Factory<T> where T : class, new() {
        public static ConcurrentDictionary<string, T> Arena = new ConcurrentDictionary<string,T>();
        private static int _id;

        // Adds a new  T2
        public T Generate(Type T2) {
            var result = T2.GetGenericInstanceOf<T>();
            var id = (++_id).ToString();
            if(!Arena.TryAdd(id, result)) {
                result = Generate(T2);
            }
            return result;
        }

        public T Generate<T2>() where T2 : new() {
            string s;
            return Generate<T2>(out s);
        }

        public T Generate<T2>([Optional] out string id) where T2 : new() {
            T result = new T2() as T;
            id = (++_id).ToString();
            if(!Arena.TryAdd(id, result)) {
                result = Generate<T2>(out id);
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