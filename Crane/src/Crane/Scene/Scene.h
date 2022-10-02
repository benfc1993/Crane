#pragma once

#include <glm/glm.hpp>
#include "entt.hpp"

namespace Crane {
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Time time);

        void OnViewportResized(uint32_t width, uint32_t height);

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        friend class Entity;
        friend class SceneSerializer;

        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    };
}