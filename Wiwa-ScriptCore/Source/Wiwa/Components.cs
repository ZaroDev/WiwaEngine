namespace Wiwa
{
    using ResourceId = System.UInt64;
    #region Transform3D
    [Component]
    public struct Transform3D
    {
        public Vector3 Position { get; private set; }
        public Vector3 Rotation { get; private set; }
        public Vector3 Scale { get; private set; }

        public Vector3 LocalPosition;
        public Vector3 LocalRotation;
        public Vector3 LocalScale;
        public Transform3D(Vector3 pos, Vector3 rot, Vector3 scl)
        {
            LocalPosition = pos;
            LocalRotation = rot;
            LocalScale = scl;

            Position = pos;
            Rotation = rot;
            Scale = scl;
        }


    }
    #endregion
    #region Mesh
    [Component]
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
    #region Utils
    [Component]
    public struct TestComponent
    {
        public int val1;
        public float val2;
    }
    #endregion
}
