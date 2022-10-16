#pragma once

#include "EditorLayer.h"
#include "UI/Panels/Panel.h"
#include "Crane/Renderer/Shader/Texture.h"


namespace Crane {
    class SceneToolbar : public Panel
    {
    public:
        template<typename F, typename G>
        SceneToolbar(F onPlay, G onStop) : OnPlay(onPlay), OnStop(onStop), Panel(true) {}

        virtual void OnImGuiRender() override;

        virtual void SetActiveScene(const Ref<Scene>& scene) { m_ActiveScene = scene; }
        virtual void SetSelectedEntity(Entity entity) {}
        virtual Entity GetSelectedEntity() const { return Entity{}; }



    private:
        Ref<Texture2D> m_PlayIcon = Texture2D::Create("resources/icons/Toolbar/PlayButton.png");
        Ref<Texture2D> m_StopIcon = Texture2D::Create("resources/icons/Toolbar/StopButton.png");

        std::function<void()> OnPlay;
        std::function<void()> OnStop;
    };
}
