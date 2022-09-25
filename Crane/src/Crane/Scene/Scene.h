#pragma once

#include <glm/glm.hpp>
#include "entt.hpp"

namespace Crane {
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());

        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Time time);

    private:
        entt::registry m_Registry;

        friend class Entity;
    };
}