using System;
using System.Collections.Generic;

namespace MQCloud.MathModeling {
    public class RequirementsBuilder : ITransitionFilterService {
        public static RequirementsBuilder Create() {
            return new RequirementsBuilder();
        }
        public static List<TransitionItem> Get<T>(int count = 1) {
            return new RequirementsBuilder().Add<T>(count).Get();
        }
        public static List<TransitionItem> Get(Type T, int count = 1) {
            return new RequirementsBuilder().Add(T, count).Get();
        }

        private List<TransitionItem> Contents;
        
        public ITransitionFilterService Add<T>(int count =1, Mode contained = Mode.Accuire) {
            Contents.Add(new TransitionItem() {
                TokenCount = count,
                ConnectionMode = contained,
                TokenType = typeof(T)
            });
            return this;
        }

        public ITransitionFilterService Add(Type T, int count = 1, Mode contained = Mode.Accuire) {
            Contents.Add(new TransitionItem() {
                TokenCount = count,
                ConnectionMode = contained,
                TokenType = T
            });
            return this;
        }

        public List<TransitionItem> Get() {
            return Contents;
        }
    }
}