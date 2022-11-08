#include "crpch.h"

#include "ScriptGlue.h"
#include "Crane/Core/UUID.h"
#include "Crane/Scene/Scene.h"
#include "Crane/Scene/Entity.h"
#include "ScriptEngine.h"
#include "Crane/Input/Input.h"
#include "Crane/Input/KeyCodes.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include <cxxabi.h>
#include <box2d/b2_body.h>

namespace Crane {

    static std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_EntityHasComponentFunctions;

#define CR_ADD_INTERNAL_CALL(name) mono_add_internal_call("Crane.InternalCalls::" #name, (const void*)&name)

    static void Print(MonoString* string)
    {
        char* CString = mono_string_to_utf8(string);
        CR_SCR_INFO(CString);
        mono_free(CString);
    }

    static void Print_Vector(glm::vec3* vector)
    {
        CR_CORE_WARN("Vector: ({0}, {1}, {2})", vector->x, vector->y, vector->z);
    }
#pragma region Entity
    static bool Entity_HasComponent(UUID entityId, MonoReflectionType* componentType)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        MonoType* type = mono_reflection_type_get_type(componentType);
        CR_CORE_ASSERT(s_EntityHasComponentFunctions.find(type) != s_EntityHasComponentFunctions.end());
        return s_EntityHasComponentFunctions.at(type)(entity);
    }
#pragma endregion

#pragma region TransformComponent
    static void TransformComponent_GetPosition(UUID entityId, glm::vec3* outTranslation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        *outTranslation = entity.GetComponent<TransformComponent>().WorldPosition;
    }

    static void TransformComponent_SetPosition(UUID entityId, glm::vec3* position)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        entity.GetComponent<TransformComponent>().WorldPosition = *position;
    }

    static void TransformComponent_GetRotation(UUID entityId, glm::vec3* outTranslation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        *outTranslation = entity.GetComponent<TransformComponent>().Rotation;
    }

    static void TransformComponent_SetRotation(UUID entityId, glm::vec3* rotation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        entity.GetComponent<TransformComponent>().Rotation = *rotation;
    }

    static void TransformComponent_GetScale(UUID entityId, glm::vec3* outTranslation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        *outTranslation = entity.GetComponent<TransformComponent>().Scale;
    }

    static void TransformComponent_SetScale(UUID entityId, glm::vec3* scale)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        entity.GetComponent<TransformComponent>().Scale = *scale;
    }
#pragma endregion

#pragma region RigidBody2DComponent

    static void RigidBody2DComponent_AddImpulse(UUID entityId, glm::vec2* impulse, glm::vec2* point, bool wake)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        auto& rb = entity.GetComponent<RigidBody2DComponent>();
        b2Body* body = (b2Body*)rb.RuntimeBody;
        body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
    }

    static void RigidBody2DComponent_AddImpulseToCenter(UUID entityId, glm::vec2* impulse, bool wake)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityId);
        CR_CORE_ASSERT(entity);

        auto& rb = entity.GetComponent<RigidBody2DComponent>();
        b2Body* body = (b2Body*)rb.RuntimeBody;
        body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
    }

#pragma endregion

#pragma region Input
    static bool Input_IsKeyDown(KeyCode keyCode)
    {
        return Input::IsKeyPressed(keyCode);
    }
#pragma endregion

    template<typename... Component>
    static void RegisterComponent()
    {
        ([&]()
        {
            int status;
            std::string typeName = abi::__cxa_demangle(typeid(Component).name(), 0, 0, &status);

            size_t pos = typeName.find_last_of(":");
            std::string structName = typeName.substr(pos + 1);
            std::string managedTypeName = fmt::format("Crane.{}", structName);
            MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetAssemblyImage());

            if (!managedType)
            {
                CR_CORE_ERROR("Could not find component type {}", managedTypeName);
                return;
            }

            s_EntityHasComponentFunctions[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
        }(), ...);
    }

    template<typename... Component>
    static void RegisterComponent(ComponentGroup<Component...>)
    {
        RegisterComponent<Component...>();
    }

    void ScriptGlue::RegisterComponents()
    {
        RegisterComponent(AllComponents{});
    }

    void ScriptGlue::RegisterFunctions()
    {
        CR_ADD_INTERNAL_CALL(Print);
        CR_ADD_INTERNAL_CALL(Print_Vector);

        CR_ADD_INTERNAL_CALL(Entity_HasComponent);

        CR_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
        CR_ADD_INTERNAL_CALL(TransformComponent_SetPosition);
        CR_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
        CR_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
        CR_ADD_INTERNAL_CALL(TransformComponent_GetScale);
        CR_ADD_INTERNAL_CALL(TransformComponent_SetScale);

        CR_ADD_INTERNAL_CALL(RigidBody2DComponent_AddImpulse);
        CR_ADD_INTERNAL_CALL(RigidBody2DComponent_AddImpulseToCenter);

        CR_ADD_INTERNAL_CALL(Input_IsKeyDown);
    }
}