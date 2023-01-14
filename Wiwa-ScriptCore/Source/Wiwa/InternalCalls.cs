using System.Runtime.CompilerServices;

namespace Wiwa
{
    using EntityId = System.UInt64;
    internal class InternalCalls
    {
        #region Log
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string log, int param);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param, out Vector3 result);
        #endregion
        #region ECS
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T GetComponent<T>(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref T AddComponent<T>(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ApplySystem(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static EntityId CreateEntity();
        #endregion
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyDownIntr(KeyCode key);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsMouseButtonPressedIntr(int button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetMouseXIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int GetMouseYIntr();
        #endregion
        #region Time
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTimeIntr();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetTimeIntr();
        #endregion
    }
}
