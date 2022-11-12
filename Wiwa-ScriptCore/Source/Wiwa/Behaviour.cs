using System;
using System.Runtime.InteropServices;

namespace Wiwa
{
    using EntityId = System.Int32;

    public class Behaviour
    {
        public virtual void OnAwake(EntityId id) { }
        public virtual void OnStart(EntityId id) { }
        public virtual void OnUpdate(EntityId id, float dt) { }
        public virtual void OnDestroy(EntityId id) { }

        public T GetComponent<T>(EntityId id) where T : unmanaged 
        {
            byte[] bytes = InternalCalls.GetComponent(id, typeof(T));
            T component = Utils.Deserialize<T>(bytes);
            return component;
        }
    }
}