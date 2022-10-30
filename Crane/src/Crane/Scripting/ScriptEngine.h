#pragma once

#include "Crane/Scene/Scene.h"
#include "Crane/Scene/Entity.h"

#include <string>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
}


namespace Crane {
    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className);

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


    private:
        std::string m_ClassNamespace;
        std::string m_ClassName;

        MonoClass* m_MonoClass = nullptr;
    };

    class ScriptInstance
    {
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass, UUID uuid);

        void InvokeOnCreate();
        void InvokeOnUpdate(float ts);

    private:
        Ref<ScriptClass> m_ScriptClass;

        MonoObject* m_Instance = nullptr;
        MonoMethod* m_Constructor = nullptr;
        MonoMethod* m_OnCreateMethod = nullptr;
        MonoMethod* m_OnUpdateMethod = nullptr;

    };

    class ScriptEngine
    {
    public:
        ScriptEngine() = default;
        static void Init();
        static void Shutdown();

        static void LoadAssembly(const std::filesystem::path& filePath);

        static void OnRuntimeStart(Scene* scene);
        static void OnRuntimeStop();

        static void OnCreateEntity(Entity entity);
        static void OnUpdateEntity(Entity entity, float ts);

        static Scene* GetSceneContext();

        static std::unordered_map<std::string, Crane::Ref<Crane::ScriptClass>> GetScripts();
        static bool ScriptClassExists(const std::string& fullClassName);
    private:
        static void InitMono();
        static void ShutdownMono();

        static void LoadAssemblyClasses(MonoAssembly* assembly);
        static MonoObject* InstantiateClass(MonoClass* monoClass);

        friend class ScriptClass;
    };

}