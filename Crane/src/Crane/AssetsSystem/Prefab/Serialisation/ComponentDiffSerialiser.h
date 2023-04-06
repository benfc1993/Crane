#pragma once

#include <yaml-cpp/yaml.h>
#include "Crane/Serialisation/YamlDecoders.h"

namespace Crane {
    class ComponentDiffDeserialiser
    {
    public:

        static YAML::Node DeserialiseComponentDiff(YAML::Node original, YAML::Node updated, std::string componentName, bool& componentChanged);
    private:
        template <typename T>
        static YAML::Node DeserialiseComponentFieldDiff(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed);

        template<typename T>
        static YAML::Node ValueDiff(YAML::Node original, YAML::Node updated, bool& changed)
        {
            auto updatedValue = updated.as<T>();
            auto originalValue = original.as<T>();

            if (updatedValue != originalValue)
            {
                changed = true;

                YAML::Node diff;
                diff["From"] = original;
                diff["To"] = updated;
                return diff;
            }

            changed = false;
            return YAML::Node();
        }
    };
}