#pragma once

#include "Crane/Camera/EditorCamera.h"
#include "Crane/Core/UUID.h"

#include <glm/glm.hpp>
#include "entt.hpp"

class b2World;

namespace Crane {
    class Entity;

    enum class SceneState
    {
        Edit = 0, Play = 1, Simulate = 2
    };

    class Scene
    {
    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(Ref<Scene> other);

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        entt::registry& Reg() { return m_Registry; }

        void OnUpdateEditor(Time time, EditorCamera& camera);

        void OnUpdateRuntime(Time time);

        void OnUpdateSimulation(Time time, EditorCamera& camera);
        void UpdatePhysics(Time time);

        void Render(Time time);

        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnSumulatePhysicsStart();
        void OnSumulatePhysicsStop();

        void OnViewportResized(uint32_t width, uint32_t height);

        void SetFilePath(const std::string& filePath) { m_FilePath = filePath; }
        std::string GetFilePath() { return m_FilePath; }

        SceneState GetState() const { return m_State; }
        void SetState(SceneState state) { m_State = state; }

        void DuplicateEntity(Entity entity);

        Entity GetPrimaryCameraEntity();

        template<typename ...Components>
        auto GetAllEntitiesWith()
        {
            return m_Registry.view<Components...>();
        }
    private:
        void SetupPhysics();
        void StopPhysics();
    private:

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        std::string m_FilePath;

        SceneState m_State = SceneState::Edit;

        b2World* m_PhysicsWorld = nullptr;

        friend class Entity;
        friend class SceneSerializer;

        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    };
}