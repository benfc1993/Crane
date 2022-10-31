#pragma once

#include "Components.h"
#include "Entity.h"

namespace Crane {
    class Hierarchy
    {
    public:
        static void AddChild(Ref<Scene> scene, UUID current, UUID child);
        static void AddChild(Ref<Scene> scene, Entity current, Entity child);
        static void AddParent(Ref<Scene> scene, Entity current, Entity child);
        static void Delete(Ref<Scene> scene, Entity entity, bool withChildren = false);
        static void Move(Ref<Scene> scene, Entity entity, Entity newParent = entt::null);
    };
}