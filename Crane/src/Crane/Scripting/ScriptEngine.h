#pragma once

#include "Crane/Scene/Scene.h"
#include "Crane/Scene/Entity.h"

#include <string>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoClassField MonoClassField;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;
    typedef struct _MonoClassField MonoClassField;
}


namespace Crane {

    enum class ScriptFieldType
    {
        None = 0,
        Float, Vector2, Vector3, Vector4,
        Int, UInt, Bool, Char, String, Double, Short, Long, Byte,
        UShort, ULong, UByte, Entity
    };

    struct ScriptField
    {
        ScriptFieldType Type;
        std::string Name;
        MonoClassField* ClassField;
    };

    struct ScriptFieldInstance
    {
        ScriptField Field;

        ScriptFieldInstance()
        {
            memset(m_Buffer, 0, sizeof(m_Buffer));
        }

        template<typename T>
        T GetValue()
        {
            static_assert(sizeof(T) <= 16, "Type too large");
            return *(T*)m_Buffer;
        }

        template<typename T>
        void SetValue(T value)
        {
            static_assert(sizeof(T) <= 16, "Type too large");
            memcpy(m_Buffer, &value, sizeof(T));
        }

    private:
        uint8_t m_Buffer[16];

        friend class ScriptInstance;
        friend class ScriptEngine;

    };

    using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

        MonoObject* Instantiate();
        MonoMethod* GetMethod(const std::string& methodName, int argCount);
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
        std::string GetName() const { return m_ClassName; }
        std::string GetFullName()
        {
            std::string fullName;
            if (m_ClassNamespace.length() != 0)
                fullName = fmt::format("{}.{}", m_ClassNamespace, m_ClassName);
            else
                fullName = m_ClassName;
            return fullName;
        }

        void SetFields();


        const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }

    private:
        std::string m_ClassNamespace;
        std::string m_ClassName;

        std::map<std::string, ScriptField> m_Fields;

        MonoClass* m_MonoClass = nullptr;

        friend class ScriptEngine;
    };

    class ScriptInstance
    {
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass, UUID uuid);

        void InvokeOnCreate();
        void InvokeOnUpdate(float ts);

        Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }
        template<typename T>
        T GetFieldValue(const std::string& name)
        {
            static_assert(sizeof(T) <= 16, "Type too large");

            bool sucess = GetFieldValueInternal(name, s_FieldValueBuffer);
            if (!sucess)
                return T();

            return *(T*)s_FieldValueBuffer;
        }

        template<typename T>
        void SetFieldValue(const std::string& name, T value)
        {
            static_assert(sizeof(T) <= 16, "Type too large");

            bool sucess = SetFieldValueInternal(name, &value);
        }

    private:
        bool GetFieldValueInternal(const std::string& name, void* buffer);
        bool SetFieldValueInternal(const std::string& name, const void* value);
    private:
        Ref<ScriptClass> m_ScriptClass;

        MonoObject* m_Instance = nullptr;
        MonoMethod* m_Constructor = nullptr;
        MonoMethod* m_OnCreateMethod = nullptr;
        MonoMethod* m_OnUpdateMethod = nullptr;

        inline static char s_FieldValueBuffer[16];

        friend struct ScriptFieldInstance;
        friend class ScriptEngine;
    };

    class ScriptEngine
    {
    public:
        ScriptEngine() = default;
        static void Init();
        static void Shutdown();

        static void LoadAssembly(const std::filesystem::path& filePath);
        static void LoadAppAssembly(const std::filesystem::path& filePath);

        static void OnRuntimeStart(Scene* scene);
        static void OnRuntimeStop();

        static void OnCreateEntity(Entity entity);
        static void OnUpdateEntity(Entity entity, float ts);

        static Scene* GetSceneContext();
        static MonoImage* GetAssemblyImage();

        static std::unordered_map<std::string, Crane::Ref<Crane::ScriptClass>> GetScripts();
        static Crane::Ref<Crane::ScriptClass> GetScript(const std::string& fullName);
        static ScriptFieldMap& GetScriptFieldMap(Entity entity);

        static bool ScriptClassExists(const std::string& fullClassName);
        static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityId);
    private:
        static void InitMono();
        static void ShutdownMono();

        static void LoadAssemblyClasses();
        static MonoObject* InstantiateClass(MonoClass* monoClass);

        friend class ScriptClass;
    };

    namespace Utils {
        inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
        {
            switch (fieldType)
            {
            case ScriptFieldType::Float: return "Float";
            case ScriptFieldType::Double: return "Double";
            case ScriptFieldType::Bool: return "Bool";
            case ScriptFieldType::Char: return "Char";
            case ScriptFieldType::String: return "String";
            case ScriptFieldType::Byte: return "Byte";
            case ScriptFieldType::Short: return "Short";
            case ScriptFieldType::Int: return "Int";
            case ScriptFieldType::Long: return "Long";
            case ScriptFieldType::UByte: return "UByte";
            case ScriptFieldType::UInt: return "UInt";
            case ScriptFieldType::UShort: return "UShort";
            case ScriptFieldType::ULong: return "ULong";
            case ScriptFieldType::Vector2: return "Vector2";
            case ScriptFieldType::Vector3: return "Vector3";
            case ScriptFieldType::Vector4: return "Vector4";
            case ScriptFieldType::Entity: return "Entity";
            }
            CR_CORE_ASSERT(false, "Unknown ScriptFieldType");
            return "None";
        }

        inline ScriptFieldType StringToScriptFieldType(std::string_view fieldType)
        {
            if (fieldType == "Float") return ScriptFieldType::Float;
            if (fieldType == "Double") return ScriptFieldType::Double;
            if (fieldType == "Bool") return ScriptFieldType::Bool;
            if (fieldType == "Char") return ScriptFieldType::Char;
            if (fieldType == "String") return ScriptFieldType::String;
            if (fieldType == "Byte") return ScriptFieldType::Byte;
            if (fieldType == "Short") return ScriptFieldType::Short;
            if (fieldType == "Int") return ScriptFieldType::Int;
            if (fieldType == "Long") return ScriptFieldType::Long;
            if (fieldType == "UByte") return ScriptFieldType::UByte;
            if (fieldType == "UInt") return ScriptFieldType::UInt;
            if (fieldType == "UShort") return ScriptFieldType::UShort;
            if (fieldType == "ULong") return ScriptFieldType::ULong;
            if (fieldType == "Vector2") return ScriptFieldType::Vector2;
            if (fieldType == "Vector3") return ScriptFieldType::Vector3;
            if (fieldType == "Vector4") return ScriptFieldType::Vector4;
            if (fieldType == "Entity") return ScriptFieldType::Entity;
            return ScriptFieldType::None;
        }
    }

}