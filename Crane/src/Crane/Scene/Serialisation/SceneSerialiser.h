#pragma once


#include "Crane/Core/Base.h"
#include "Crane/Scene/Scene.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
	class SceneSerialiser
	{
	public:
		SceneSerialiser(const Ref<Scene>& scene);

		void Serialise(const std::string filePath);
		void SerialiseRuntime(const std::string& filePath);

		bool Deserialise(const std::string filePath);
		bool DeserialiseRuntime(const std::string& filePath);

		void SerialiseEntity(YAML::Emitter& out, Entity entity);
		Entity DeserialiseEntity(YAML::Node entity, bool prefab = false);
	private:
		Ref<Scene> m_scene;
	};
}