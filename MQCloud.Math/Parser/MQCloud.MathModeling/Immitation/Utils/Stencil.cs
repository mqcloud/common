using System.Linq;
using MQCloud.MathModeling.Immitation.Interfaces;

namespace MQCloud.MathModeling.Immitation.Utils {
    public class Stencil<T>: IStencilCtx<T> where T : new() {
        private readonly StencilArray<T> _parent;
        private readonly int[] _current;

        public Stencil(StencilArray<T> parent, params int[] current) {
            _parent = parent;
            _current = current;
        }

        public T GetCurrent() {
            return _parent.Get(_current);
        }

        public T GetRelativeToCurrent(params int[] dimentions) {
            var position = _current.Select(i => i).ForEach((i, id) => {
                id += dimentions[i];
            }).ToArray();
            return _parent.Get(position);
        }
    }
}