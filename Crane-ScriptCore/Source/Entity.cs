using System;

namespace Crane
{

    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }
    public class Entity
    {
        public readonly ulong id;

        protected Entity() { id = 0; }

        public Entity(ulong id)
        {
            this.id = id;
        }

        public Vector3 Position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(id, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(id, ref value);
            }
        }
    }
}
