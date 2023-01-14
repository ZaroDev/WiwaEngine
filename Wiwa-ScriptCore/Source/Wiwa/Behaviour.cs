using System;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Wiwa
{
    using EntityId = System.UInt64;

    public class Behaviour
    {
        public ref T GetComponent<T>(EntityId id) where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(id, typeof(T));
        }

        public ref T AddComponent<T>(EntityId id) where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(id, typeof(T));
        }
    }
}