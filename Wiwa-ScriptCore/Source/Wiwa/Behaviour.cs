namespace Wiwa
{
    using EntityId = System.UInt64;

    public class Behaviour
    {
        public T GetComponent<T>(EntityId id) where T : unmanaged 
        {
            byte[] bytes = InternalCalls.GetComponent(id, typeof(T));
            T component = Utils.Deserialize<T>(bytes);
            return component;
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