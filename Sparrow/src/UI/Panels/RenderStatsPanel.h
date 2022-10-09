#pragma once

#include "Panel.h"

namespace Crane {
    class RenderStatsPanel : public Panel
    {
    public:
        RenderStatsPanel(bool isRequired = false) : Panel(isRequired) {}
        virtual void OnImGuiRender() override;
    };
}