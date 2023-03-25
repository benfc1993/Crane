using System;

namespace Crane
{
    public class Component
    {
        public Entity entity { get; internal set; }
    }


    public class TransformComponent : Component
    {
        public Vector3 position
        {
            get
            {
                InternalCalls.TransformComponent_GetPosition(entity.Id, out Vector3 value);
                return value;
            }
            set
            {
                InternalCalls.TransformComponent_SetPosition(entity.Id, ref value);
            }
        }

        public Vector3 rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(entity.Id, out Vector3 value);
                return value;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(entity.Id, ref value);
            }
        }

        public Vector3 scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(entity.Id, out Vector3 value);
                return value;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(entity.Id, ref value);
            }
        }
    }

    public class RigidBody2DComponent : Component
    {
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 point, bool wake)
        {
            InternalCalls.RigidBody2DComponent_AddImpulse(entity.Id, ref impulse, ref point, wake);
        }
        public void ApplyLinearImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_AddImpulseToCenter(entity.Id, ref impulse, wake);
        }
    }

    public class CameraComponent : Component
    {
    }

    public class ScriptComponent : Component { }

}