#pragma once 

#include "Crane/Scene/Entity.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
	class ComponentSerialiser
	{
	public:
		template <typename T>
		static void SerialiseComponent(YAML::Emitter& out, Entity entity);
	};
}