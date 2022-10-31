#include "crpch.h"

#include "Hierarchy.h"

namespace Crane {

    static Entity GetLastChild(Ref<Scene> scene, UUID first)
    {
        Entity curr = scene->GetEntityByUUID(first);
        while (!curr.IsNull())
        {
            UUID next = curr.GetComponent<HierarchyComponent>().Next;
            if (next == 0) break;
            curr = scene->GetEntityByUUID(next);
        }

        return curr;
    }

    void Hierarchy::AddChild(Ref<Scene> scene, UUID parent, UUID child)
    {
        AddChild(scene, scene->GetEntityByUUID(parent), scene->GetEntityByUUID(child));
    }
    void Hierarchy::AddChild(Ref<Scene> scene, Entity parent, Entity child)
    {
        auto& parentCom = parent.GetComponent<HierarchyComponent>();
        auto& childCom = child.GetComponent<HierarchyComponent>();
        childCom.Parent = parent.GetUUID();

        if (parentCom.First == 0)
        {
            parentCom.First = child.GetUUID();
            return;
        }

        Entity lastChild = GetLastChild(scene, parentCom.First);
        lastChild.GetComponent<HierarchyComponent>().Next = child.GetUUID();
        childCom.Prev = lastChild.GetUUID();

    }

    void Hierarchy::AddParent(Ref<Scene> scene, Entity current, Entity newParent)
    {
        auto& currentCom = current.GetComponent<HierarchyComponent>();
        auto& newParentCom = newParent.GetComponent<HierarchyComponent>();

        newParentCom.First = current.GetUUID();

        if (currentCom.Parent != 0)
        {
            Delete(scene, current);
            AddChild(scene, scene->GetEntityByUUID(currentCom.Parent), newParent);
        }

        currentCom.Parent = newParent.GetUUID();

    }
    void Hierarchy::Delete(Ref<Scene> scene, Entity entity, bool withChildren)
    {
        auto& entityCom = entity.GetComponent<HierarchyComponent>();

        if (entityCom.Next != 0)
            scene->GetEntityByUUID(entityCom.Next).GetComponent<HierarchyComponent>().Prev = entityCom.Prev;

        if (entityCom.Prev != 0)
            scene->GetEntityByUUID(entityCom.Prev).GetComponent<HierarchyComponent>().Next = entityCom.Next;

        if (entityCom.Parent != 0)
        {
            auto& parentHc = scene->GetEntityByUUID(entityCom.Parent).GetComponent<HierarchyComponent>();
            if (parentHc.First == entity.GetUUID())
                parentHc.First = entityCom.Next;
        }

        if (withChildren || entityCom.First == 0) return;


        UUID curr = entityCom.First;
        while (curr != 0)
        {
            auto& com = scene->GetEntityByUUID(curr).GetComponent<HierarchyComponent>();
            UUID next = com.Next;

            if (entityCom.Parent == 0)
            {
                com.Parent = 0;
                com.Prev = 0;
                com.Next = 0;
            }
            else
            {
                AddChild(scene, entityCom.Parent, curr);
            }

            curr = next;
        }

    }
    void Hierarchy::Move(Ref<Scene> scene, Entity entity, Entity newParent)
    {
        Delete(scene, entity, true);
        auto& hc = entity.GetComponent<HierarchyComponent>();
        hc.Parent = 0;
        hc.Prev = 0;
        hc.Next = 0;

        if (!newParent.IsNull())
            AddChild(scene, newParent, entity);
    }
}