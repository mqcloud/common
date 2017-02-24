using System.Collections.Generic;
using MQCloud.MathModeling.Immitation.Places;

namespace MQCloud.MathModeling.Immitation.Interfaces {
    public interface ITaggable<T> {
        T SetTag(Tag tag);
        T RemoveTag(Tag tag);
        List<string> GetTags();
    }
}