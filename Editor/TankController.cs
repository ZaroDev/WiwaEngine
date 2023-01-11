using Wiwa;

namespace WiwaApp
{
    using EntityId = System.UInt64;
    [Component]
    struct TankComponent
    {
        Transform3D camera;
        Transform3D tankTower;
        Transform3D tankCannon;
        EntityId bulletPrefab;
        Transform3D spawnPoint;

        float rotationX;
        float rotationY;
        float velY;
        float velX;
        float upperLookLimit;
        float lowerLookLimit;
        float tankVelocity;
        float rotateVelocity;
        float fireInterval;
        float nextFire;
    }

    class TankController : Behaviour
    {
        void Awake(EntityId entityId)
        {

        }

        void Init(EntityId entityId)
        {

        }

        void Update(EntityId entityId)
        {
            ref TankComponent tank = ref GetComponent<TankComponent>(entityId);

            CameraControls();

            var transAmount = tankVelocity * Time.deltaTime;
            var rotAmount = rotateVelocity * Time.deltaTime;
            if (Input.GetKey(KeyCode.W))
            {
                tank.Translate(0, 0, transAmount);
            }
            else if (Input.GetKey(KeyCode.S))
            {
                tank.Translate(0, 0, transAmount);
            }
            if (Input.GetKey(KeyCode.A))
            {
                tank.Rotate(0, -rotAmount, 0);
            }
            else if (Input.GetKey(KeyCode.D))
            {
                tank.Rotate(0, rotAmount, 0);
            }
            if (Input.GetMouseButtonDown(0) && Time.time > nextFire)
            {
                nextFire = Time.time + fireInterval;
                var bullet = Instantiate(bulletPrefab, spawnPoint.position, spawnPoint.rotation);
            }
        }

        private void CameraControls()
        {
            rotationX -= Input.GetAxis("Mouse Y") * velY;
            rotationY -= Input.GetAxis("Mouse X") * velX;
            rotationX = Mathf.Clamp(rotationX, -upperLookLimit, lowerLookLimit);
            tankTower.localRotation = Quaternion.Euler(0, rotationY, 0);
            tankCannon.localRotation = Quaternion.Euler(rotationX, 0, 0);
            camera.LookAt(tankTower);
        }
    }
}
