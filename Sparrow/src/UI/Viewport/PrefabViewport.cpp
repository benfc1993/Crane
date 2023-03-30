#include "PrefabViewport.h"

#include "Crane/Core/Application.h"

namespace Crane {
	PrefabViewport::PrefabViewport(EditorLayer* editorLayer, std::string prefabName, UUID prefabHandle): m_PrefabHandle(prefabHandle), EditorViewport(prefabName, CreateRef<Scene>(), editorLayer)
	{
		auto prefabAsset = Application::Get().GetAssetRegistry()->GetAsset(prefabHandle);
		m_Prefab = PrefabSerialiser::DeserialisePrefab(prefabAsset.FilePath, m_Scene, true);
	}

	void PrefabViewport::OnUpdate(Time time)
	{
		m_ActiveScene->OnUpdateEditor(time);
		EditorViewport::OnUpdate(time);
	}
	void PrefabViewport::OnSavePrefab()
	{
		auto prefabAsset = Application::Get().GetAssetRegistry()->GetAsset(m_PrefabHandle);

		PrefabSerialiser::SerialisePrefab(m_Scene, m_Prefab, prefabAsset.FilePath);
		Application::Get().GetAssetRegistry()->OnAssetChanged(m_PrefabHandle);
	}
}