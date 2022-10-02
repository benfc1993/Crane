#pragma once 

#include "Crane/Scene/Entity.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
    class ComponentDeserializer
    {
    public:
        template <typename T>
        static void DeserializeComponent(YAML::Node& data, Entity& entity);
    };
}