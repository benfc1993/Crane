using System.Runtime.CompilerServices;

namespace Crane {
    public static class Debug {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static string Print(string msg);
        public static void Log(string msg)
        {
            Print(msg);
        }
    }
}