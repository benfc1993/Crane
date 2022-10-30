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
    }

    public struct Transform
    {
        public ulong id;

        public Transform(ulong id) { this.id = id; }
        public Vector3 position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(id, out Vector3 value);
                return value;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(id, ref value);
            }
        }

        public Vector3 rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(id, out Vector3 value);
                return value;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(id, ref value);
            }
        }

        public Vector3 scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(id, out Vector3 value);
                return value;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(id, ref value);
            }
        }
    }

    public class Entity
    {
        public readonly ulong Id;

        protected Transform transform;

        protected Entity() { Id = 0; }

        public Entity(ulong id)
        {
            this.Id = id;
            transform = new Transform(id);
        }


    }
}
