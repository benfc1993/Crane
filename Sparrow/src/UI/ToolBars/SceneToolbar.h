#pragma once

#include "UI/Panels/Panel.h"
#include "Crane/Renderer/Shader/Texture.h"

namespace Crane {
    class SceneToolbar : public Panel
    {
    public:

        SceneToolbar() : Panel(true) {}

        virtual void OnImGuiRender() override;

        virtual void SetActiveScene(const Ref<Scene>& scene) { m_ActiveScene = scene; }
        virtual void SetSelectedEntity(Entity entity) {}
        virtual Entity GetSelectedEntity() const { return Entity{}; }

    private:
        Ref<Texture2D> m_PlayIcon = Texture2D::Create("resources/icons/Toolbar/PlayButton.png");
        Ref<Texture2D> m_StopIcon = Texture2D::Create("resources/icons/Toolbar/StopButton.png");
    };
}