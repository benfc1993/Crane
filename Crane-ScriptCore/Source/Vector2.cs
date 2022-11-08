using System;

namespace Crane
{
    public struct Vector2
    {
        public float x, y;

        public static Vector2 zero => new Vector2(0, 0);

        public Vector2(float scalar)
        {
            this.x = scalar;
            this.y = scalar;
        }

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public static explicit operator Vector2(Vector3 v) => new Vector2(v.x, v.y);

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.x + b.x, a.y + b.y);
        }

        public static Vector2 operator *(Vector2 a, float b)
        {
            return new Vector2(a.x * b, a.y * b);
        }
    }
}