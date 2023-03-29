#pragma once

#include "ImGuizmoLayer.h"
#include "Viewport.h"
#include "EditorLayer.h"
#include "Crane/Camera/EditorCamera.h"


namespace Crane {
	class EditorViewport: public Viewport
	{
	public:
		EditorViewport(std::string name, Ref<Scene> scene, EditorLayer* editorLayer);
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Time time) override;
		virtual void OnEvent(Event& event) override;
		virtual void OnSceneStateChanged(Ref<Scene> scene) override;
		void OnOverlayRender();
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool HandleInput(KeyPressedEvent& event);

	private:
		void DrawImGuizmo(Entity selectedEntity);

	private:
		EditorCamera m_camera;

		EditorLayer* m_editorLayer;
		Ref<ImGuizmoLayer> m_imGuizmoLayer;

		Entity m_HoveredEntity;

		bool m_CanPick = false;

		int m_GizmoType = 7;
		int m_GizmoSpace = 0;
	};
}