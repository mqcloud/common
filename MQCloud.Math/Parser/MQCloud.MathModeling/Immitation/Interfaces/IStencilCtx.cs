namespace MQCloud.MathModeling.Immitation.Interfaces {
    public interface IStencilCtx<T> {
        T GetCurrent();
        T GetRelativeToCurrent(params int[] dimentions);
    }
}