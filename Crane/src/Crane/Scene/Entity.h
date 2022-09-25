#pragma once

#include "entt.hpp"
#include "Scene.h"


namespace Crane {
    class Entity
    {
    public:
        friend class Scene;
        Entity() {}
        Entity(entt::entity entity, Scene* scene) : m_EntityHandle(entity), m_Scene(scene) {};
        Entity(const Entity& other) = default;

        template <typename T, typename ...Args>
        T& AddComponent(Args&&... args)
        {
            CR_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        };

        template <typename T, typename ...Args>
        T& AddOrReplaceComponent(Args&&... args)
        {
            return m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
        };

        template <typename T>
        T& GetComponent()
        {
            CR_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        };

        template <typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        };

        template <typename T>
        void RemoveComponent()
        {
            CR_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
            return m_Scene->m_Registry.remove<T>(m_EntityHandle);
        };

        operator bool() const { return (uint32_t)m_EntityHandle != 0; }

    private:
        entt::entity m_EntityHandle{ 0 };
        Scene* m_Scene = nullptr;
    };
}