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
            bool sucess = GetFieldValueInternal(name, s_FieldValueBuffer);
            if (!sucess)
                return T();

            return *(T*)s_FieldValueBuffer;
        }

        template<typename T>
        void SetFieldValue(const std::string& name, const T& value)
        {
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

        inline static char s_FieldValueBuffer[8];
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

        static bool ScriptClassExists(const std::string& fullClassName);
        static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityId);
    private:
        static void InitMono();
        static void ShutdownMono();

        static void LoadAssemblyClasses();
        static MonoObject* InstantiateClass(MonoClass* monoClass);

        friend class ScriptClass;
    };

}