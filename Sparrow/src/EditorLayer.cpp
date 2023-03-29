#include "EditorLayer.h"
#include "imgui/imgui_demo.cpp"
#include "Crane/Scene/ScriptableEntity.h"
#include "Crane/Utils/PlatformUtils.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"
#include "Crane/Math/Math.h"
#include "UI/Viewport/EditorViewport.h"
#include "UI/Viewport/RuntimeViewport.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sys/stat.h>

namespace Crane
{
	extern const std::filesystem::path s_AssetPath = "assets";

	EditorLayer::EditorLayer(): Layer("EditorLayer")
	{
		m_EditorSettings->Deserialize();
	}
	EditorLayer::~EditorLayer()
	{
		m_EditorSettings->Serialize();
	}

	void EditorLayer::OnAttach()
	{

		auto scenePath = Project::GetActive()->GetScenePath();

		if (scenePath.empty())
		{
			NewScene();
		}
		else
		{
			LoadScene(scenePath);
		}


		m_Viewports.AddViewport<EditorViewport>("Scene", m_ActiveScene, (EditorLayer*)this);
		m_Viewports.AddViewport<RuntimeViewport>("Game", m_ActiveScene);

		m_Panels.AddPanel<SceneHierarchyPanel>(m_ActiveScene, true);

		m_SettingsPanel = CreateRef<SettingsPanel>(true);
		m_Panels.AddPanel(m_SettingsPanel);

		m_Panels.AddPanel<RenderStatsPanel>();

		m_Panels.AddPanel<ContentBrowserPanel>((void*)this, true);

		m_Panels.AddPanel<SceneToolbar>(m_EditorSettings, std::bind(&EditorLayer::OnScenePlay, this), std::bind(&EditorLayer::OnSceneStop, this), std::bind(&EditorLayer::OnSimulateStart, this), std::bind(&EditorLayer::OnSimulateStop, this));

		for (auto viewport : m_Viewports.GetViewports())
		{
			m_Panels.Push((Ref<Panel>&)viewport);
		}

		m_Panels.SetActiveScene(m_ActiveScene);
	}
	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Time time)
	{
		switch (m_SceneState)
		{
		case SceneState::Edit:
			m_ActiveScene->OnUpdateEditor(time);
			break;
		case SceneState::Simulate:
			m_ActiveScene->OnUpdateSimulation(time);
			break;
		case SceneState::Play:
			m_ActiveScene->OnUpdateRuntime(time);
			break;
		}
		m_Viewports.OnUpdate(time);
	}

	void EditorLayer::OnImGuiRender()
	{
		CR_PROFILE_FUNCTION();


		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;



		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dockspace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGui::ShowDemoWindow();
		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minWindowSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 150.0f;

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		style.WindowMinSize.x = minWindowSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Preferences"))
					m_SettingsPanel->OpenPanel();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveScene();
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("New Project"))
				{
					//TODO: Provide form for name and location
					Project::New("NewProject", "/home/ben/projects");
					LoadScene(Project::GetActive()->GetScenePath());
					m_Panels.OnProjectChanged();
				}

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Panels"))
			{
				if (ImGui::MenuItem("Add Hierarchy Panel"))
					m_Panels.AddPanel<SceneHierarchyPanel>(m_ActiveScene);

				if (ImGui::MenuItem("Add RenderStats Panel"))
					m_Panels.AddPanel<RenderStatsPanel>();
				if (ImGui::MenuItem("Add Content Browser Panel"))
					m_Panels.AddPanel<ContentBrowserPanel>((void*)this);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		m_Panels.OnImGuiRender();

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(CR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		m_Viewports.OnEvent(event);
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
			if (control)
			{
				NewScene();
				return true;
			}
		case Key::O:
			if (control)
			{
				OpenScene();
				return true;
			}
		case Key::S:
			if (control)
			{
				SaveScene();
				return true;
			}
			if (control && shift)
			{
				SaveSceneAs();
				return true;
			}
		case Key::D:
			if (control)
			{
				OnDuplicateEntity();
				return true;
			}
		case Key::P:
			if (control)
			{
				if (m_SceneState == SceneState::Edit)
				{
					OnScenePlay();
					return true;
				}
				if (m_SceneState == SceneState::Play)
				{
					OnSceneStop();
					return true;
				}
			}

		default:
			return false;
		}

	}

	void EditorLayer::NewScene()
	{
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;
		m_Panels.SetActiveScene(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::filesystem::path filePath = FileDialogs::OpenFile();
		LoadScene(filePath);

	}

	void EditorLayer::LoadScene(const std::filesystem::path& path)
	{

		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		if (path.extension() != ".scene")
		{
			CR_WARN("Could not load {0} - Not a scene file", path.filename().string());
			return;
		}

		if (!path.empty())
		{
			Ref<Scene> newScene = CreateRef<Scene>();
			SceneSerializer serializer(newScene);
			if (serializer.Deserialize(path))
			{
				m_EditorScene = newScene;
				m_ActiveScene = m_EditorScene;
				m_ActiveScene->SetFilePath(path);
				m_Panels.SetActiveScene(m_ActiveScene);
			}
		}
	}
	void EditorLayer::SaveScene()
	{
		struct stat buffer;
		int fileExists = stat(m_ActiveScene->GetFilePath().c_str(), &buffer);

		if (fileExists == 0)
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(m_ActiveScene->GetFilePath());
		}
		else
		{
			SaveSceneAs();
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDialogs::SaveFile();
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filePath);
			m_ActiveScene->SetFilePath(filePath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;


		uint64_t selectedId = -1;
		auto selectedEntity = m_Panels.GetSelectedEntity();

		if (!selectedEntity.IsNull())
		{
			selectedId = selectedEntity.GetUUID();
		}

		m_ActiveScene = Scene::Copy(m_EditorScene);

		m_ActiveScene->OnViewportResized(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->OnRuntimeStart();
		m_ActiveScene->SetState(SceneState::Play);

		m_Panels.SetActiveScene(m_ActiveScene);

		m_Viewports.OnSceneStateChanged(m_ActiveScene);

		if (selectedId != -1)
		{
			m_Panels.SetSelectedEntity(m_ActiveScene->GetEntityByUUID(selectedId));
		}
	}

	void EditorLayer::OnSceneStop()
	{

		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnRuntimeStop();

		uint64_t selectedId = -1;
		auto selectedEntity = m_Panels.GetSelectedEntity();

		if (!selectedEntity.IsNull())
		{
			selectedId = selectedEntity.GetUUID();
		}

		m_ActiveScene = m_EditorScene;
		m_ActiveScene->SetState(SceneState::Edit);

		m_Panels.SetActiveScene(m_ActiveScene);

		m_Viewports.OnSceneStateChanged(m_ActiveScene);

		if (selectedId != -1)
		{
			m_Panels.SetSelectedEntity(m_ActiveScene->GetEntityByUUID(selectedId));
		}

	}

	void EditorLayer::OnSimulateStart()
	{


		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		uint64_t selectedId = -1;
		auto selectedEntity = m_Panels.GetSelectedEntity();

		m_ActiveScene = Scene::Copy(m_EditorScene);

		m_ActiveScene->OnViewportResized(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->OnSumulatePhysicsStart();
		m_ActiveScene->SetState(SceneState::Simulate);
		m_Panels.SetActiveScene(m_ActiveScene);

		m_Viewports.OnSceneStateChanged(m_ActiveScene);

		if (!selectedEntity.IsNull())
		{
			selectedId = selectedEntity.GetUUID();
		}
	}

	void EditorLayer::OnSimulateStop()
	{

		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnSumulatePhysicsStop();

		uint64_t selectedId = -1;
		auto selectedEntity = m_Panels.GetSelectedEntity();

		m_ActiveScene = m_EditorScene;
		m_ActiveScene->SetState(SceneState::Edit);
		m_Panels.SetActiveScene(m_ActiveScene);

		m_Viewports.OnSceneStateChanged(m_ActiveScene);

		if (!selectedEntity.IsNull())
		{
			selectedId = selectedEntity.GetUUID();
		}
	}

	void EditorLayer::OnDuplicateEntity()
	{
		Entity selectedEntity = m_Panels.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}
}