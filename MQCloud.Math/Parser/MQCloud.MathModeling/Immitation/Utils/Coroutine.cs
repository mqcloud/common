using System.Collections.Generic;

namespace MQCloud.MathModeling.Immitation.Utils {
    public abstract class Coroutine {
        private readonly IEnumerator<object> _enumerator;
        private bool _doSub;
        private Coroutine _subCoroutine;
        private object _input;
        private object _subInput;
        private bool _isComplete;
        private bool _canMove = true;

        public Coroutine() {
            _enumerator = Process().GetEnumerator();
        }

        public object Activate(Coroutine coroutine, object sub_input = null) {
            _doSub = true;
            _subCoroutine = coroutine;
            _subInput = sub_input;
            return _subCoroutine.next();
        }

        public object Complete(object return_value = null) {
            _isComplete = true;
            return return_value;
        }

        public object next(object in_value = null) {
            if (_doSub) {
                if (_subCoroutine._canMove && !_subCoroutine._isComplete) {
                    return _subCoroutine.next(_subInput);
                }
                _doSub = false;
                _input = in_value;
                _canMove = _enumerator.MoveNext();
                return _enumerator.Current;
            }
            _input = in_value;
            _canMove = _enumerator.MoveNext();
            return _enumerator.Current;
        }

        public void initialize(object input) {
            _input = input;
        }

        public abstract IEnumerable<object> Process();
    }
}