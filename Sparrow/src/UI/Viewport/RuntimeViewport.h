#pragma once

#include "Viewport.h"

#include "Crane/Scene/Entity.h"

namespace Crane {
	class RuntimeViewport: public Viewport
	{
	public:
		RuntimeViewport(std::string name, Ref<Scene> scene);
		virtual void OnUpdate(Time time) override;
		virtual void OnImGuiRender() override;
		virtual void OnSceneStateChanged(Ref<Scene> scene) override;
	private:
		Entity m_PrimaryCamera;
	};
}