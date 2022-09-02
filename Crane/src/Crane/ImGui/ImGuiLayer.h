#pragma once

#include "Crane/Core/Layer.h"

namespace Crane
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event &event);

    private:
        float m_Time = 0.0f;
    };
}