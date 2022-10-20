#pragma once

#include "entt.hpp"
#include "Components.h"
#include "Scene.h"

#include "Crane/Core/UUID.h"

namespace Crane {
    // struct TransformComponent;
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
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
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
        bool TryGetComponent(T& t)
        {
            t = m_Scene->m_Registry.get<T>(m_EntityHandle);
            return HasComponent<T>();
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
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        };

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        UUID GetUUID() { return  GetComponent<IdComponent>().Id; }
        const std::string& GetName() { return  GetComponent<TagComponent>().Tag; }

        bool operator!=(const Entity& other)
        {
            return !(*this == other);
        }

        bool operator==(const Entity& other) const
        {
            return (m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene);
        }

    private:
        entt::entity m_EntityHandle{ entt::null };
        Scene* m_Scene = nullptr;
    };
}