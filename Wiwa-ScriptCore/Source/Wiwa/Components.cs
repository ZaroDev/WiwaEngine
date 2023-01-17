namespace Wiwa
{
    using ResourceId = System.UInt64;
    #region Transform3D
    [Component]
    public struct Transform3D
    {
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale;

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
    //[Component]
    //public unsafe struct Mesh
    //{
    //    public ResourceId meshId;
    //    public ResourceId materialId;
    //    public bool drawChildren;
    //    public ulong modelIndex;
    //    public fixed char mesh_path[128];
    //    public fixed char mat_path[128];
    //}
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