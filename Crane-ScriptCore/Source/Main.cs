using System;

namespace Crane
{

    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }
    public class Main
    {


        public float FloatVar { get; set; }

        public Main()
        {
            Vector3 pos = new Vector3(1.0f, 5.2f, 4.7f);
            Debug.Log(pos);
        }

        public void PrintMessage(string message)
        {
            Debug.Log($"C# says: {message}");
        }

    }
}
