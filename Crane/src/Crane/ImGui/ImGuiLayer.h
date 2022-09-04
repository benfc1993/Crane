#pragma once

#include "Crane/Core/Layer.h"

#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"
#include "Crane/Events/ApplicationEvent.h"

namespace Crane
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}