using System.Collections.Generic;

namespace MQCloud.MathModeling.Immitation.Places {
    public class Scope {
        private readonly string _name;
        private Scope _parent;
        private List<Scope> children;

        public string Name {
            get {
                string result = null;
                if (_parent != null) {
                    result = _parent.Name;
                }
                result += _name;
                return result;
            }
        }

        public Scope(string name, List<Scope> children = null) {
            _name = name;
            this.children = children;
        }

        public void Nest(Scope other) {
            other.children.Add(this);
            this._parent = other;
        }

        public void Collect(Scope other) {
            this.children.Add(other);
            other._parent = this;
        }
    }
}