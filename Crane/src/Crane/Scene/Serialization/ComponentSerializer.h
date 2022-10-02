#pragma once 

#include "Crane/Scene/Entity.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
    class ComponentSerializer
    {
    public:
        template <typename T>
        static void SerializeComponent(YAML::Emitter& out, Entity entity);
    };
}