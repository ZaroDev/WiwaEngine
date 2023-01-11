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

        public T AddComponent<T>(EntityId id) where T : unmanaged
        {
            byte[] bytes = InternalCalls.AddComponent(id, typeof(T), null);
            T component = Utils.Deserialize<T>(bytes);
            return component;
        }
        public T AddComponent<T>(EntityId id, T component) where T : unmanaged
        {
            byte[] bytes = Utils.Serialize(component);
            InternalCalls.AddComponent(id, typeof(T), bytes);
            return component;
        }
    }
}