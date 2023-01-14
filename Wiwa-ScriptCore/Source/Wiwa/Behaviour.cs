using System;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Wiwa
{
    using EntityId = System.UInt64;

    public class Behaviour
    {
        protected EntityId m_EntityId;
        public ref T GetComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(m_EntityId, typeof(T));
        }

        public ref T AddComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(m_EntityId, typeof(T));
        }
    }
}