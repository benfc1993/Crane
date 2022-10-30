using System;
using Crane;

namespace Sandbox
{
    public class Player : Entity
    {
        float speed = 3.2f;
        void OnCreate()
        {
            Debug.Log($"Player.OnCreate");
        }

        void OnUpdate(float ts)
        {
            Vector3 velocity = Vector3.zero;

            Debug.Log($"Player.OnUpdate {ts}");

            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.y += speed;
            }

            if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.y -= speed;
            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.x -= speed;
            }

            if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.x += speed;
            }

            transform.position += velocity * ts;
        }
    }
}