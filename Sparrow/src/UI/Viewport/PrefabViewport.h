#pragma once

#include "EditorViewport.h"

#include "Crane/Core/UUID.h"

namespace Crane {
	class PrefabViewport: public EditorViewport
	{
	public:
		PrefabViewport(EditorLayer* editorLayer, std::string prefabName, UUID prefabHandle);
		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Time time) override;

	private:
		void OnSavePrefab();

	private:
		UUID m_PrefabHandle;
		Entity m_Prefab;

		//TODO: implement auto save
	};
}