#pragma once

#include "Crane/Camera/EditorCamera.h"

#include <glm/glm.hpp>
#include "entt.hpp"

namespace Crane {
    class Entity;

    enum class SceneState
    {
        Edit = 0, Play = 1
    };

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        entt::registry& Reg() { return m_Registry; }

        void OnUpdateEditor(Time time, EditorCamera& camera);

        void OnUpdateRuntime(Time time);

        void OnViewportResized(uint32_t width, uint32_t height);

        void SetFilePath(const std::string& filePath) { m_FilePath = filePath; }
        std::string GetFilePath() { return m_FilePath; }

        SceneState GetState() const { return m_State; }
        void SetState(SceneState state) { m_State = state; }

        Entity GetPrimaryCameraEntity();
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        std::string m_FilePath;

        SceneState m_State = SceneState::Edit;

        friend class Entity;
        friend class SceneSerializer;

        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    };
}