using System;

namespace Crane
{
    public class Entity
    {
        public readonly ulong Id;

        protected TransformComponent transform;

        protected Entity() { Id = 0; }

        public Entity(ulong id)
        {
            this.Id = id;
            transform = new TransformComponent() { entity = this };
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(Id, type);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { entity = this };
            return component;
        }

        public Entity FindEntityByName(string name)
        {
            ulong entityID = InternalCalls.Entity_FindEntityByName(name);
            if (entityID == 0) return null;
            return new Entity(entityID);
        }
    }

}
