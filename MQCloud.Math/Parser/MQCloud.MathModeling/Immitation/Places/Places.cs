using MQCloud.MathModeling.Immitation.Utils;

namespace MQCloud.MathModeling.Immitation.Places {
    public class Places : StencilArray<Place> {
        private int _id;

        public Places(params int[] dimentions) : base(dimentions) {}

        protected override Place DefaultConstructor() {
            return Runtime.Simulation.CreatePlace(_id++.ToString());
        }

        protected override Place AbsentElementConstructor() {
            return Runtime.Simulation.CreatePlace(_id++.ToString()).SetTag(Tag.Bad);
        }
    }
}