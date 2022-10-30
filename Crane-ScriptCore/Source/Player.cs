using System;
using Crane;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Debug.Log($"Player.OnCreate");
        }

        void OnUpdate(float ts)
        {
            Debug.Log($"Player.OnUpdate {ts}");
            Vector3 pos = Position;
            pos.x = Position.x + 10 * ts;

            Position = pos;
        }
    }
}