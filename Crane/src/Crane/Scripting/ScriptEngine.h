#pragma once

#include <string>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
}


namespace Crane {
    class ScriptEngine
    {
    public:
        ScriptEngine() = default;
        static void Init();
        static void Shutdown();
    private:
        static void InitMono();
        static void ShutdownMono();

        static void LoadAssembly(const std::filesystem::path& filePath);
        static void LoadAssemblyClasses(MonoAssembly* assembly);
        static MonoObject* InstantiateClass(MonoClass* monoClass);

        friend class ScriptClass;
    };

    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className);

        MonoObject* Instantiate();
        MonoMethod* GetMethod(const std::string& methodName, int argCount);
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

    private:
        std::string m_ClassNamespace;
        std::string m_ClassName;

        MonoClass* m_MonoClass = nullptr;
    };

    class ScriptInstance
    {
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass);

        void InvokeOnCreate();
        void InvokeOnUpdate(float ts);

    private:
        Ref<ScriptClass> m_ScriptClass;

        MonoObject* m_Instance = nullptr;
        MonoMethod* m_OnCreateMethod = nullptr;
        MonoMethod* m_OnUpdateMethod = nullptr;

    };
}