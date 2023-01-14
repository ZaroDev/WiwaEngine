using System;

namespace Wiwa
{
    using EntityId = System.UInt64;
    class EnemyController : Behaviour
    {
        void Awake()
        {
            ref TestComponent tc = ref AddComponent<TestComponent>();
            tc.val1 = (int)m_EntityId;

            EntityId entity = CreateEntity();
            AddComponent<Transform3D>(entity);
            ref TestComponent test = ref AddComponent<TestComponent>(entity);
            test.val1 = (int)entity;
        }

        void Init()
        {
            Console.WriteLine("Init: {0}", m_EntityId.ToString());
        }

        void Update()
        {
            Console.WriteLine("Update: {0}", m_EntityId.ToString());
        }
    }
}