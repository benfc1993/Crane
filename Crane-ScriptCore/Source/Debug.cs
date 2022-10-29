namespace Crane
{
    public static class Debug
    {


        public static void Log(string msg)
        {
            InternalCalls.Print(msg);
        }

        public static void Log(Vector3 vec)
        {
            InternalCalls.Print_Vector(ref vec);
        }
    }
}