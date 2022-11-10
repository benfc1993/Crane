using System;
using Crane;

namespace Sandbox
{
    public class Player : Entity
    {
        public float speed = 20.0f;
        private RigidBody2DComponent rigidBody;

        void OnCreate()
        {
            Debug.Log($"Player.OnCreate");
            rigidBody = GetComponent<RigidBody2DComponent>();
        }

        void OnUpdate(float ts)
        {
            Debug.Log($"ts: {ts}");
            Vector2 velocity = Vector2.zero;

            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.y += speed * ts;
            }

            if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.y -= speed * ts;
            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.x -= speed * ts;
            }

            if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.x += speed * ts;
            }

            // transform.position += velocity * ts;
            if (rigidBody != null)
                rigidBody.ApplyLinearImpulse(velocity, true);
        }
    }
}