#pragma once 

#include "Crane/Scene/Entity.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
	class ComponentDeserialiser
	{
	public:
		template <typename T>
		static void DeserialiseComponent(YAML::Node& data, Entity& entity);
	};
}