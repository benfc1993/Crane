#include "crpch.h"

#include "ScriptGlue.h"

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

    void ScriptGlue::RegisterFunctions()
    {
        CR_ADD_INTERNAL_CALL(Print);
        CR_ADD_INTERNAL_CALL(Print_Vector);
    }
}