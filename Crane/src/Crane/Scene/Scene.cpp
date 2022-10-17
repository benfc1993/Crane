#include "crpch.h"

#include "Scene.h"

#include "Components.h"
#include "ScriptableEntity.h"
#include "Crane/Renderer/Renderer2D.h"

#include "Entity.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Crane {

    static b2BodyType CrBodyTypeToB2BodyType(RigidBody2DComponent::BodyType bodyType)
    {
        switch (bodyType)
        {
        case RigidBody2DComponent::BodyType::Static: return b2_staticBody;
        case RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
        case RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }

        CR_CORE_ASSERT(false, "Invalid body type");
        return b2_staticBody;
    }

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {

    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IdComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IdComponent>(uuid);
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        entity.AddComponent<TransformComponent>();
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

            Renderer2D::DrawSprite(transform.Transform(), sprite, (int)entity);
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

        {
            const uint32_t velocityIterations = 6;
            const uint32_t positionIterations = 2;
            m_PhysicsWorld->Step(time.DeltaTime(), velocityIterations, positionIterations);

            auto view = m_Registry.view<RigidBody2DComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

                b2Body* body = (b2Body*)rb2d.RuntimeBody;
                const auto& position = body->GetPosition();
                transform.Position.x = position.x;
                transform.Position.y = position.y;
                transform.Rotation.z = body->GetAngle();
            }
        }

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
            Renderer2D::BeginScene(*mainCamera, cameraTransform->Transform());
            auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : spriteGroup)
            {
                auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.Transform(), sprite.Color);
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

    void Scene::OnRuntimeStart()
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.81f });
        {
            auto view = m_Registry.view<RigidBody2DComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

                b2BodyDef bodyDef;
                bodyDef.type = CrBodyTypeToB2BodyType(rb2d.Type);
                bodyDef.fixedRotation = rb2d.FixedRotation;
                bodyDef.position.Set(transform.Position.x, transform.Position.y);
                bodyDef.angle = transform.Rotation.z;

                b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
                rb2d.RuntimeBody = body;
            };
        }

        {
            auto view = m_Registry.view<BoxCollider2DComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();

                b2PolygonShape polygonShape;
                polygonShape.SetAsBox(boxCollider.Size.x * transform.Scale.x, boxCollider.Size.y * transform.Scale.y);

                RigidBody2DComponent component;
                if (entity.TryGetComponent(component))
                {
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &polygonShape;
                    fixtureDef.density = boxCollider.Density;
                    fixtureDef.friction = boxCollider.Friction;
                    fixtureDef.restitution = boxCollider.Restitution;
                    fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;
#
                    ((b2Body*)component.RuntimeBody)->CreateFixture(&fixtureDef);
                }
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
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
    void Scene::OnComponentAdded<IdComponent>(Entity entity, IdComponent& component)
    {
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
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
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

    template<>
    void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
    {
    }
}

