using System;
using Crane;

namespace Sandbox
{
    public class Player : Entity
    {
        float speed = 0.2f;
        RigidBody2DComponent rigidBody;
        void OnCreate()
        {
            Debug.Log($"Player.OnCreate");
            rigidBody = GetComponent<RigidBody2DComponent>();
        }

        void OnUpdate(float ts)
        {
            Vector2 velocity = Vector2.zero;

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

            // transform.position += velocity * ts;
            if (rigidBody != null)
                rigidBody.ApplyLinearImpulse(velocity, true);
        }
    }
}