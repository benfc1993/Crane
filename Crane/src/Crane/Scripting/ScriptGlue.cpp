#include "crpch.h"

#include "ScriptGlue.h"
#include "Crane/Core/UUID.h"
#include "Crane/Scene/Scene.h"
#include "Crane/Scene/Entity.h"
#include "ScriptEngine.h"

#include "mono/metadata/object.h"

namespace Crane {
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

    static void TransformComponent_GetPosition(UUID entityID, glm::vec3* outTranslation)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityID);
        CR_CORE_ASSERT(entity);

        *outTranslation = entity.GetComponent<TransformComponent>().Position;
    }

    static void TransformComponent_SetPosition(UUID entityID, glm::vec3* position)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        CR_CORE_ASSERT(scene);
        Entity entity = scene->GetEntityByUUID(entityID);
        CR_CORE_ASSERT(entity);

        entity.GetComponent<TransformComponent>().Position = *position;
    }

    void ScriptGlue::RegisterFunctions()
    {
        CR_ADD_INTERNAL_CALL(Print);
        CR_ADD_INTERNAL_CALL(Print_Vector);
        CR_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
        CR_ADD_INTERNAL_CALL(TransformComponent_SetPosition);
    }
}