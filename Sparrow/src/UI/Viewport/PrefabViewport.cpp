#include "PrefabViewport.h"

#include "Crane/AssetsSystem/Prefab/Prefab.h"
#include "Crane/AssetsSystem/Prefab/Serialisation/PrefabSerialiser.h"

#include "Crane/Core/Application.h"

namespace Crane {
	PrefabViewport::PrefabViewport(EditorLayer* editorLayer, std::string prefabName, UUID prefabHandle): m_PrefabHandle(prefabHandle), EditorViewport(prefabName, CreateRef<Scene>(), editorLayer)
	{
		auto prefabAsset = Application::Get().GetAssetRegistry()->GetAsset(prefabHandle);
		m_Prefab = PrefabSerialiser::DeserialisePrefab(prefabAsset.FilePath, m_Scene, true, true);
	}

	void PrefabViewport::OnUpdate(Time time)
	{
		m_Scene->OnUpdateEditor(time);
		EditorViewport::OnUpdate(time);
	}

	void PrefabViewport::OnImGuiRender()
	{
		ImGui::BeginMenuBar();

		if (ImGui::Button("Save"))
		{
			OnSavePrefab();
		}

		ImGui::EndMenuBar();
		EditorViewport::OnImGuiRender();
	}

	void PrefabViewport::OnSavePrefab()
	{
		auto prefabAsset = Application::Get().GetAssetRegistry()->GetAsset(m_PrefabHandle);

		Prefab::GetDiff(m_Scene, m_PrefabHandle, m_Prefab);
		PrefabSerialiser::SerialisePrefab(m_Scene, m_Prefab, prefabAsset.FilePath, true);
		Application::Get().GetAssetRegistry()->OnAssetChanged(m_PrefabHandle);
	}
}