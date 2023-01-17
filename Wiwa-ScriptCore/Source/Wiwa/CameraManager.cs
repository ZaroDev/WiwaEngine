namespace Wiwa
{
    using CameraId = System.UInt64;
    public class CameraManager
    {
        public static CameraId GetActiveCamera()
        {
            return InternalCalls.GetActiveCamera();
        }

        public static void SetPosition(CameraId camid, Vector3 pos)
        {
            InternalCalls.SetCameraPosition(camid, pos);
        }

        public static void LookAt(CameraId camid, Vector3 pos)
        {
            InternalCalls.CameraLookAt(camid, pos);
        }

        public static void SetFront(CameraId camid, Vector3 front)
        {
            InternalCalls.SetCameraFront(camid, front);
        }

        public static void SetCameraRotation(CameraId camid, Vector3 rot)
        {
            InternalCalls.SetCameraRotation(camid, rot);
        }
    }
}
