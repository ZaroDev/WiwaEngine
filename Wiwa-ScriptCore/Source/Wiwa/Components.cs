using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace Wiwa
{
    using ResourceId = System.UInt64;
    #region Transform
    public struct Transform
    {
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale;

        public Transform(Vector3 pos, Vector3 rot, Vector3 scl)
        {
            Position = pos;
            Rotation = rot;
            Scale = scl;
        }
    }
    #endregion
    #region Mesh
    public struct Mesh
    {
        public ResourceId MeshId;
        public ResourceId MaterialId;

        public Mesh(ResourceId meshId, ResourceId matId)
        {
            MeshId = meshId;
            MaterialId = matId;
        }
    }
    #endregion
}
