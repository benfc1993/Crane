#pragma once 

#include "Scene.h"
#include "Entity.h"

#include "Crane/Core/Base.h"

namespace Crane {
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {}

        // template<typename T>
        // T& GetComponent()
        // {
        //     return m_Entity.GetComponent<T>();
        // }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(Time time) {}
    private:
        Entity m_Entity;
        friend class Scene;
    };
}