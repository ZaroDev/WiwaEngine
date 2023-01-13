using System;

namespace Wiwa
{
    using EntityId = System.UInt64;
    class EnemyController : Behaviour
    {
        void Awake(EntityId entityId)
        {
            //Console.WriteLine("Awaking: {0}", entityId.ToString());

            //ref Transform3D t3d = ref GetComponent<Transform3D>(entityId);

            //Console.WriteLine("Transform3D -> Position: ({0},{1},{2}) Rotation: ({3},{4},{5}), Scale: ({6},{7},{8})",
            //    t3d.LocalPosition.x, t3d.LocalPosition.y, t3d.LocalPosition.z,
            //    t3d.LocalRotation.x, t3d.LocalRotation.y, t3d.LocalRotation.z,
            //    t3d.LocalScale.x, t3d.LocalScale.y, t3d.LocalScale.z
            //);

            //t3d.LocalPosition.x += 1;
        }

        void Init(EntityId entityId)
        {
            //Console.WriteLine("Init: {0}", entityId.ToString());

            //ref Transform3D t3d = ref GetComponent<Transform3D>(entityId);

            //Console.WriteLine("Transform3D -> Position: ({0},{1},{2}) Rotation: ({3},{4},{5}), Scale: ({6},{7},{8})",
            //    t3d.LocalPosition.x, t3d.LocalPosition.y, t3d.LocalPosition.z,
            //    t3d.LocalRotation.x, t3d.LocalRotation.y, t3d.LocalRotation.z,
            //    t3d.LocalScale.x, t3d.LocalScale.y, t3d.LocalScale.z
            //);

            //t3d.LocalPosition.x += 1;
        }

        void Update(EntityId entityId)
        {
            //Console.WriteLine("Updating: {0}", entityId.ToString());

            //ref Transform3D t3d = ref GetComponent<Transform3D>(entityId);

            //Console.WriteLine("Transform3D -> Position: ({0},{1},{2}) Rotation: ({3},{4},{5}), Scale: ({6},{7},{8})",
            //    t3d.LocalPosition.x, t3d.LocalPosition.y, t3d.LocalPosition.z,
            //    t3d.LocalRotation.x, t3d.LocalRotation.y, t3d.LocalRotation.z,
            //    t3d.LocalScale.x, t3d.LocalScale.y, t3d.LocalScale.z
            //);

            //t3d.LocalPosition.x += 1;
        }
    }
}