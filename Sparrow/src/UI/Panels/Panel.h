#pragma once


#include "Crane/Core/Base.h"
#include "Crane/Scene/Scene.h"
#include "Crane/Scene/Entity.h"

namespace Crane {
    class Panel
    {
    public:
        Panel(bool isRequired = false): m_Required(isRequired) {}
        bool IsRequired() { return m_Required; }
        virtual void OnImGuiRender() = 0;
        virtual void OnProjectChanged() {};

        void SetIndex(int index) { m_Index = index; }
        virtual void SetActiveScene(const Ref<Scene>& scene) { m_ActiveScene = scene; }
        void SetSelectedEntity(Entity entity)
        {
            m_SetSelectedEntity(entity);
        };

        virtual void OnSelectedEntityChanged(Entity entity) {}

        void SetSelectionFunction(std::function<void(Entity entity)> func) { m_SetSelectedEntity = func; }

        virtual Entity GetSelectedEntity() const { return Entity{}; }
    protected:
        bool m_Required;
        Ref<Scene> m_ActiveScene = nullptr;
        int m_Index = -1;
        std::function<void(Entity entity)> m_SetSelectedEntity;
    };
}