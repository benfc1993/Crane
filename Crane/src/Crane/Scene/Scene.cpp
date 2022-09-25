#include "crpch.h"

#include "Scene.h"

#include "Components.h"
#include "Crane/Renderer/Renderer2D.h"

#include "Entity.h"

namespace Crane {
    Scene::Scene()
    {

    }

    Scene::~Scene()
    {

    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(Time time)
    {
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        auto view = m_Registry.view <TransformComponent, CameraComponent>();
        for (auto entity : view)
        {
            const auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            if (camera.primary)
            {
                mainCamera = &camera.Camera;
                cameraTransform = &transform.Transform;
                break;
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, *cameraTransform);
            auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : spriteGroup)
            {
                const auto& [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform, sprite.Color);
            }
            Renderer2D::EndScene();
        }

    }

}

