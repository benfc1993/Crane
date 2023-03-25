using System;
using System.Collections.Generic;

namespace Crane
{
    public class Entity
    {
        public readonly ulong Id;

        protected TransformComponent transform;

        protected Entity() { Id = 0; transform = new TransformComponent() { entity = this }; }
        public Entity(ulong id)
        {
            this.Id = id;
            transform = new TransformComponent() { entity = this };
        }
        protected Entity? parent
        {
            get
            {
                var parent = InternalCalls.Entity_GetParent(Id);
                if (parent == 0) return null;
                return new Entity(parent);
            }
        }

        protected Entity[]? children
        {
            get
            {

                var childId = InternalCalls.Entity_GetChild(Id);


                if (childId == 0) return null;

                List<Entity> result = new List<Entity>() { new Entity(childId) };

                while (childId != 0)
                {
                    childId = InternalCalls.Entity_GetSibling(childId);
                    if (childId == 0) break;
                    result.Add(new Entity(childId));
                }

                return result.ToArray();
            }
        }


        public bool HasComponent<T>() where T : Component, new()
        {

            Type type = typeof(T);
            return InternalCalls.Entity_HasComponent(Id, type);
        }

        public T? GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { entity = this };
            return component;
        }

        public T? GetComponentInChildren<T>() where T : Entity, new()
        {
            if (children?.Length == 0) return null;

            for (int i = 0; i < children?.Length; i++)
            {
                if (children[i].HasComponent<ScriptComponent>())
                {
                    Type type = typeof(T);
                    T classScript = InternalCalls.Entity_GetComponentClass<T>(children[i].Id, type);
                    if (classScript != null)
                        return classScript;
                }
            }

            return null;
        }

        public T? GetComponentInParent<T>() where T : Component, new()
        {
            if (parent == null) return null;

            if (parent.HasComponent<T>())
            {
                Type type = typeof(T);
                return parent.GetComponent<T>();
            }

            return null;
        }

        public T? GetScriptInParent<T>() where T : Entity, new()
        {
            if (parent == null) return null;

            if (parent.HasComponent<ScriptComponent>())
            {
                Type type = typeof(T);
                T classScript = InternalCalls.Entity_GetComponentClass<T>(parent.Id, type);
                if (classScript != null)
                    return classScript;
            }

            return null;
        }

        public Entity? FindEntityByName(string name)
        {
            ulong entityID = InternalCalls.Entity_FindEntityByName(name);
            if (entityID == 0) return null;
            return new Entity(entityID);
        }
    }

}
