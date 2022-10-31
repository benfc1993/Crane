#include "crpch.h"

#include "Scene.h"

#include "Components.h"
#include "ScriptableEntity.h"
#include "Crane/Renderer/Renderer2D.h"
#include "Crane/Scripting/ScriptEngine.h"

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
        StopPhysics();
    }
#pragma region templates
    template<typename... Component>
    static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        ([&]()
        {
            auto view = src.view<Component>();
            for (auto srcEntity : view)
            {
                entt::entity dstEntity = enttMap.at(src.get<IdComponent>(srcEntity).Id);

                auto& srcComponent = src.get<Component>(srcEntity);
                dst.emplace_or_replace<Component>(dstEntity, srcComponent);
            }
        }(), ...);
    }

    template<typename... Component>
    static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        CopyComponent<Component...>(dst, src, enttMap);
    }

    template<typename... Component>
    static void CopyComponentIfExists(Entity dst, Entity src)
    {
        ([&]()
        {
            if (src.HasComponent<Component>())
                dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
        }(), ...);
    }

    template<typename... Component>
    static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
    {
        CopyComponentIfExists<Component...>(dst, src);
    }

    template<typename Component>
    static void CopyComponent(entt::registry& dst, entt::registry& src, std::unordered_map<UUID, entt::entity>& idMap)
    {
        auto view = src.view<Component>();

        for (auto e : view)
        {
            UUID uuid = src.get<IdComponent>(e).Id;
            CR_CORE_ASSERT(idMap.find(uuid) != idMap.end());
            entt::entity enttId = idMap.at(uuid);

            auto& component = src.get<Component>(e);

            dst.emplace_or_replace<Component>(enttId, component);
        }
    }

    template<typename Component>
    static void CopyComponentIfExists(Entity dst, Entity src)
    {
        if (src.HasComponent<Component>())
        {
            dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
        }
    }
#pragma endregion

#pragma region Entity
    Ref<Scene> Scene::Copy(Ref<Scene> other)
    {
        Ref<Scene> newScene = CreateRef<Scene>();

        newScene->m_ViewportWidth = other->m_ViewportWidth;
        newScene->m_ViewportHeight = other->m_ViewportHeight;

        auto& srcSceneReg = other->m_Registry;
        auto& dstSceneReg = newScene->m_Registry;
        auto idView = srcSceneReg.view<IdComponent>();

        std::unordered_map<UUID, entt::entity> idMap;

        for (auto e : idView)
        {
            UUID uuid = srcSceneReg.get<IdComponent>(e).Id;
            const auto& name = srcSceneReg.get<TagComponent>(e).Tag;
            Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
            idMap[uuid] = newEntity;
        }

        CopyComponent<HierarchyComponent>(dstSceneReg, srcSceneReg, idMap);

        CopyComponent(AllComponents{}, dstSceneReg, srcSceneReg, idMap);

        return newScene;
    }


    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IdComponent>(uuid);
        entity.AddComponent<HierarchyComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        entity.AddComponent<TransformComponent>();
        m_EntityMap[uuid] = entity;

        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_EntityMap.erase(entity.GetUUID());
        m_Registry.destroy(entity);
    }

    bool Scene::EntityExists(UUID uuid)
    {
        return m_EntityMap.find(uuid) != m_EntityMap.end();
    }

    void Scene::DuplicateEntity(Entity entity)
    {
        Entity newEntity = CreateEntity(entity.GetName());
        CopyComponentIfExists(AllComponents{}, newEntity, entity);
    }

    Entity Scene::GetEntityByUUID(UUID uuid)
    {
        if (m_EntityMap.find(uuid) != m_EntityMap.end())
            return { m_EntityMap.at(uuid), this };

        return {};
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
#pragma endregion

    void Scene::OnRuntimeStart()
    {
        SetupPhysics();

        ScriptEngine::OnRuntimeStart(this);

        auto view = m_Registry.view <ScriptComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };
            const auto sc = entity.GetComponent<ScriptComponent>();

            if (ScriptEngine::ScriptClassExists(sc.FullName))
            {
                ScriptEngine::OnCreateEntity(entity);
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        StopPhysics();
    }

    void Scene::OnSumulatePhysicsStart()
    {
        SetupPhysics();
    }

    void Scene::OnSumulatePhysicsStop()
    {
        StopPhysics();
    }

    void Scene::OnUpdateEditor(Time time, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        Render(time);
        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(Time time)
    {

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

        // UpdateScripts
        m_Registry.view<ScriptComponent>().each([=](auto e, auto& sc)
        {
            Entity entity = { e, this };
            ScriptEngine::OnUpdateEntity(entity, time.DeltaTime());
        });



        UpdatePhysics(time);

        Camera* mainCamera = nullptr;
        TransformComponent* cameraTransform = nullptr;

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
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

            Render(time);

            Renderer2D::EndScene();
        }

    }


    void Scene::OnUpdateSimulation(Time time, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        UpdatePhysics(time);
        Render(time);
        Renderer2D::EndScene();
    }


    void Scene::SetupPhysics()
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
                polygonShape.SetAsBox(boxCollider.Size.x * transform.Scale.x, boxCollider.Size.y * transform.Scale.y, b2Vec2(boxCollider.Offset.x, boxCollider.Offset.y), 0.0f);

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

        {
            auto view = m_Registry.view<CircleColliderComponent>();
            for (auto e : view)
            {
                Entity entity = { e, this };
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& circleCollider = entity.GetComponent<CircleColliderComponent>();

                b2CircleShape circleShape;
                circleShape.m_p.Set(circleCollider.Offset.x, circleCollider.Offset.y);
                circleShape.m_radius = circleCollider.Radius * transform.Scale.x;

                RigidBody2DComponent component;
                if (entity.TryGetComponent(component))
                {
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &circleShape;
                    fixtureDef.density = circleCollider.Density;
                    fixtureDef.friction = circleCollider.Friction;
                    fixtureDef.restitution = circleCollider.Restitution;
                    fixtureDef.restitutionThreshold = circleCollider.RestitutionThreshold;
#
                    ((b2Body*)component.RuntimeBody)->CreateFixture(&fixtureDef);
                }
            }
        }
    }

    void Scene::StopPhysics()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    void Scene::UpdatePhysics(Time time)
    {
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
    }


    void Scene::Render(Time time)
    {
        auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : spriteGroup)
        {
            auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform.Transform(), sprite.Color, (int)entity);
        }

        auto circleView = m_Registry.view<TransformComponent, CircleRendererComponent>();
        for (auto entity : circleView)
        {
            auto [transform, circle] = circleView.get<TransformComponent, CircleRendererComponent>(entity);

            Renderer2D::DrawCircle(transform.Transform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
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

#pragma region OnComponentAdded
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
    void Scene::OnComponentAdded<HierarchyComponent>(Entity entity, HierarchyComponent& component)
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
    void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
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

    template<>
    void Scene::OnComponentAdded<CircleColliderComponent>(Entity entity, CircleColliderComponent& component)
    {
    }
    template<>
    void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
    {
    }

#pragma endregion
}

