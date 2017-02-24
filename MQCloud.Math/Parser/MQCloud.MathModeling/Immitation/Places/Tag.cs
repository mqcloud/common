using System;

namespace MQCloud.MathModeling.Immitation.Places {
    public class Tag : IEquatable<Tag> {
        public static Tag Bad = GetTag("Bad");

        private Tag(string name) {
            Name = name;
        }

        public string Name { get; set; }

        public static Tag GetTag(string name) {
            return new Tag(name);
        } 

        public override int GetHashCode() {
            return Name.GetHashCode();
        }

        public override bool Equals(object obj) {
            var otherTag = obj as Tag;
            return Equals(otherTag);
        }

        private bool Equals(Tag other) {
            return Name.Equals(other?.Name);
        }

        bool IEquatable<Tag>.Equals(Tag other) {
            return Equals(other);
        }
    }
}