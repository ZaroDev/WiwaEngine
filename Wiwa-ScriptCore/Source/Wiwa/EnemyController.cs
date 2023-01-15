using System;

namespace Wiwa
{
    using EntityId = System.UInt64;

    class EnemyController : Behaviour
    {
        //URef<Transform3D> transform;
        void Awake()
        {
            //transform = new URef<Transform3D>(ref GetComponent<Transform3D>());

            //transform.Get().LocalPosition.x += 3;

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

            //ref Transform3D t3d = ref GetComponent<Transform3D>();

            //Console.WriteLine(t3d.Position.x);
        }
    }
}