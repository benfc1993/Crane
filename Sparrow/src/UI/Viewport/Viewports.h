#pragma once

#include "Crane/Core/Base.h"
#include "Viewport.h"

namespace Crane {
    class Viewports
    {
    public:
        void OnImGuiRender()
        {
            for (auto viewport : m_Viewports)
            {
                viewport->OnImGuiRender();
            }
        }

        void OnUpdate(Time time)
        {
            for (auto viewport : m_Viewports)
            {
                viewport->OnUpdate(time);
            }
        }

        void OnEvent(Event& event)
        {
            for (auto viewport : m_Viewports)
            {
                viewport->OnEvent(event);
            }
        }



        template <typename T, typename ...Args>
        void AddViewport(Args&&... args)
        {
            auto viewport = CreateRef<T>(std::forward<Args>(args)...);
            AddViewport(viewport);
        }

        void AddViewport(const Ref<Viewport>& viewport)
        {
            m_Viewports.push_back(viewport);
            viewport->SetIndex(m_Viewports.size() - 1);
        }

        void RemoveViewport(const int index)
        {
            auto viewport = m_Viewports[index];
            m_Viewports.erase(m_Viewports.begin() + index);
        }

        std::vector<Ref<Viewport>> GetViewports() { return m_Viewports; }
    private:
        std::vector<Ref<Viewport>> m_Viewports;
    };
}