using System;

namespace MQCloud.MathModeling {
    public enum Mode {
        Accuire = 0,
        Ignore,
        CheckContained,
        CheckMissing
    }
    public class TransitionItem {
       
        public Type TokenType;
        public int TokenCount;
        public Mode ConnectionMode;


    }
    public class TransitionItem<T>: TransitionItem {
        TransitionItem() {
            this.TokenType = typeof(T);
        }

        public static TransitionItem<T> GetItem(int count) {
            return new TransitionItem<T>() { TokenCount = count };
        }
    }
}