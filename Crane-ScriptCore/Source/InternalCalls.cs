using System.Runtime.CompilerServices;

namespace Crane
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Print(string msg);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Print_Vector(ref Vector3 param);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TransformComponent_GetPosition(ulong entityID, out Vector3 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TransformComponent_SetPosition(ulong entityID, ref Vector3 position);
    }
}