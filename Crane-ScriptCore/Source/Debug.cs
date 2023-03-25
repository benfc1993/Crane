namespace Crane
{
    public static class Debug
    {
        public static void Log(object msg)
        {
            InternalCalls.Print(msg.ToString());
        }
    }
}