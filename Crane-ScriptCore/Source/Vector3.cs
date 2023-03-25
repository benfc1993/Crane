using System;

namespace Crane
{
    public struct Vector3
    {
        public float x, y, z;

        public static Vector3 zero => new Vector3(0, 0, 0);

        public Vector3(float scalar)
        {
            this.x = scalar;
            this.y = scalar;
            this.z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator *(Vector3 a, float b)
        {
            return new Vector3(a.x * b, a.y * b, a.z * b);
        }

        public override string ToString()
        {
            return $"x: {x}, y: {y}, z: {z}";
        }
    }
}

