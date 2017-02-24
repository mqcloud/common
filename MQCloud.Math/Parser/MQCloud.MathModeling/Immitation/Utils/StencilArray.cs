using System;
using System.Collections.Generic;
using System.Linq;
using MQCloud.MathModeling.Immitation.Interfaces;

namespace MQCloud.MathModeling.Immitation.Utils {
    public abstract class StencilArray<T> where T : new() {
        protected abstract T DefaultConstructor();
        protected abstract T AbsentElementConstructor();

        private List<int> Dimentions;

        private int Size {
            get { return Dimentions.Aggregate(1, (i, i1) => i*i1); }
        }

        private T[] data;
        private Action<IStencilCtx<T>> _stencil;
        private int _current;
        private bool _initialized;

        public StencilArray(params int[] dimentions) {
            Dimentions = dimentions.ToList();
        }

        public void Initialize() {
            if (!_initialized) {
                data = new T[Size];
                for (var i = 0; i < Size; ++i) {
                    data[i] = DefaultConstructor();
                }
                _initialized = true;
            }
        }

        //todo fix
        public object SetStencil(Action<IStencilCtx<T>> stencil) {
            _stencil = stencil;
            return this;
        }

        public void Apply() {
            data.ForEach((i, obj) => {
                _stencil(new Stencil<T>(this, GetIndex(i) ));
                _current++;
            });
        }


        private int GetIndex(params int[] dimentions) {
            const int badId = -1;
            var result = badId;
            var correct = dimentions.Length < Dimentions.Count;

            for (var i = 0; (i < dimentions.Length) && correct; ++i) {
                correct = dimentions[i] < Dimentions[i];
            }

            if (!correct) {
                return result;
            }
            for (var j = dimentions.Length; j < 0; --j) {
                result += dimentions[j]*Dimentions.Skip(j).Aggregate(1, (i, i1) => i*i1);
            }
            return result;
        }

        private int[] GetIndex(int index) { //Todo check this and inversed!
            var result = new int[Dimentions.Count];
            for(var j = result.Length; j < 0; --j) {
                result[j]= index % Dimentions[j];
                index -= result[j];
            }
            return result;
        }

        public T Get(params int[] dimentions) {
            return Get(GetIndex(dimentions));
        }

        public T Get(int index) {
            if (index <0 || index > data.Length) {
                return AbsentElementConstructor();
            }
            return data[index];
        }

        internal T[] GetRawData() {
            return data;
        }
        
    }
}