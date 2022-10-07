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

        void SetFilePath(const std::string& filePath) { m_FilePath = filePath; }
        std::string GetFilePath() { return m_FilePath; }

        Entity GetPrimaryCameraEntity();
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        std::string m_FilePath;

        friend class Entity;
        friend class SceneSerializer;

        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    };
}