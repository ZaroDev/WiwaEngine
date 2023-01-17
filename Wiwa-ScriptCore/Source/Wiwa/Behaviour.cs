namespace Wiwa
{
    using EntityId = System.UInt64;

    public class Behaviour
    {
        protected EntityId m_EntityId;

        public ref T GetComponent<T>(EntityId entity) where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(entity, typeof(T));
        }

        public ref T AddComponent<T>(EntityId entity) where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(entity, typeof(T));
        }
        public void ApplySystem<T>(EntityId entity)
        {
            InternalCalls.ApplySystem(entity, typeof(T));
        }
        public void DestroyEntity(EntityId entity)
        {
            InternalCalls.DestroyEntity(entity);
        }

        public ref T GetComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.GetComponent<T>(m_EntityId, typeof(T));
        }

        public ref T AddComponent<T>() where T : unmanaged
        {
            return ref InternalCalls.AddComponent<T>(m_EntityId, typeof(T));
        }

        public void ApplySystem<T>()
        {
            InternalCalls.ApplySystem(m_EntityId, typeof(T));
        }
        public void DestroyEntity()
        {
            InternalCalls.DestroyEntity(m_EntityId);
        }

        public EntityId CreateEntity()
        {
            EntityId id = InternalCalls.CreateEntity();
            AddComponent<Transform3D>(id);
            return id;
        }

        public static void AddMesh(EntityId eid, string model, string material)
        {
            InternalCalls.AddMeshToEntity(eid, model, material);
        }
    }
}