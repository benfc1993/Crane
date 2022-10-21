#pragma once

#include "EditorLayer.h"
#include "UI/Panels/Panel.h"
#include "Crane/Renderer/Shader/Texture.h"

#include "EditorSettings.h"


namespace Crane {
    class SceneToolbar : public Panel
    {
    public:
        template<typename F, typename G, typename H, typename I>
        SceneToolbar(Ref<EditorSettings> editorSettings, F onPlay, G onStop, H onSimulateStart, I onSimulateStop) : m_EditorSettings(editorSettings), OnPlay(onPlay), OnStop(onStop), OnSimulateStart(onSimulateStart), OnSimulateStop(onSimulateStop), Panel(true) {}

        virtual void OnImGuiRender() override;

        virtual void SetActiveScene(const Ref<Scene>& scene) { m_ActiveScene = scene; }
        virtual void SetSelectedEntity(Entity entity) {}
        virtual Entity GetSelectedEntity() const { return Entity{}; }



    private:
        Ref<Texture2D> m_PlayIcon = Texture2D::Create("resources/icons/Toolbar/PlayButton.png");
        Ref<Texture2D> m_StopIcon = Texture2D::Create("resources/icons/Toolbar/StopButton.png");
        Ref<Texture2D> m_SimulateIcon = Texture2D::Create("resources/icons/Toolbar/SimulateButton.png");

        std::function<void()> OnPlay;
        std::function<void()> OnStop;
        std::function<void()> OnSimulateStart;
        std::function<void()> OnSimulateStop;

        Ref<EditorSettings> m_EditorSettings;
    };
}
