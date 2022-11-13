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
    }
}