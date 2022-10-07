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

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdateEditor(Time time, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : spriteGroup)
        {
            auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }

        {
            auto view = m_Registry.view<ParticleSystemComponent, TransformComponent>();
            for (auto entity : view)
            {
                auto [particleSystem, transform] = view.get<ParticleSystemComponent, TransformComponent>(entity);

                particleSystem.Data.Position = transform.Position;

                for (int i = 0; i < particleSystem.Data.BurstSize; i++)
                {
                    particleSystem.System.Emit(particleSystem.Data);
                }

                particleSystem.System.OnUpdate(time);
                particleSystem.System.OnRender();
            }

        }

        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(Time time)
    {

        //Update script
        //TODO: Move to Scene on play
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            if (!nsc.Instance)
            {
                nsc.Instance = nsc.InstatiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }

            nsc.Instance->OnUpdate(time);
        });



        Camera* mainCamera = nullptr;
        TransformComponent* cameraTransform = nullptr;

        {
            auto view = m_Registry.view <TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform;
                    break;
                }
            }
        }

        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, *cameraTransform);
            auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : spriteGroup)
            {
                auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform, sprite.Color);
            }

            {
                auto view = m_Registry.view<ParticleSystemComponent, TransformComponent>();
                for (auto entity : view)
                {
                    auto [particleSystem, transform] = view.get<ParticleSystemComponent, TransformComponent>(entity);

                    particleSystem.Data.Position = transform.Position;

                    for (int i = 0; i < particleSystem.Data.BurstSize; i++)
                    {
                        particleSystem.System.Emit(particleSystem.Data);
                    }

                    particleSystem.System.OnUpdate(time);
                    particleSystem.System.OnRender();
                }

            }

            Renderer2D::EndScene();
        }

    }
    void Scene::OnViewportResized(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view <CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_Registry.view <CameraComponent>();
        for (auto entity : view)
        {
            auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
                return Entity{ entity, this };
        }

        return {};
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        CR_ASSERT(true, "OnComponentAdded not handled");
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<ParticleSystemComponent>(Entity entity, ParticleSystemComponent& component)
    {
    }
}

