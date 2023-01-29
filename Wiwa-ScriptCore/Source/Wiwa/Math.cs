using System;

namespace Wiwa
{
    public static class Math
    {
        public static Vector3 forward = new Vector3(0f, 0f, 1f);
        public static Vector3 right = new Vector3(1f, 0f, 0f);
        public static Vector3 up = new Vector3(0f, 1f, 0f);
        #region Trigonometry
        public static T Clamp<T>(this T val, T min, T max) where T : IComparable<T>
        {
            if (val.CompareTo(min) < 0) return min;
            else if (val.CompareTo(max) > 0) return max;
            else return val;
        }
        public static double Radians(double degrees)
        {
            return degrees * System.Math.PI / 180f;
        }
        public static double Cos(double value)
        {
            return System.Math.Cos(value);
        }
        public static float Cos(float value)
        {
            return (float)Cos((double)value);
        }
        public static double Sin(double value)
        {
            return System.Math.Sin(value);
        }
        public static float Sin(float value)
        {
            return (float)Sin((double)value);
        }
        public static double Sqrt(double value)
        {
            return System.Math.Sqrt(value);
        }
        public static double Atan2(double x, double y)
        {
            return System.Math.Atan2(x, y);
        }
        public static Vector3 Normalize(Vector3 A)
        {
            double distance = System.Math.Sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
            return new Vector3((float)(A.y / distance), (float)(A.y / distance), (float)(A.z / distance));
        }
        public static Vector3 CrossProduct(Vector3 v1, Vector3 v2)
        {
            float x, y, z;
            x = v1.y * v2.z - v2.y * v1.z;
            y = (v1.x * v2.z - v2.x * v1.z) * -1;
            z = v1.x * v2.y - v2.x * v1.y;

            return new Vector3(x, y, z);
        }
        #endregion

        #region Transforms
        //X Pitch
        //Y Roll
        //Z Yaw
        public static Vector3 CalculateUp(ref Transform3D t3d)
        {
            Vector3 rotrad;
            rotrad.x = Math.DegToRad(t3d.Rotation.x);
            rotrad.y = Math.DegToRad(t3d.Rotation.y);
            rotrad.z = Math.DegToRad(t3d.Rotation.z);

            Vector3 up;
            up.x = Sin(rotrad.x) * Sin(rotrad.z);
            up.y = Cos(rotrad.x);
            up.z = Sin(rotrad.x) * Cos(rotrad.z);

            up.x = Math.RadToDeg(up.x);
            up.y = Math.RadToDeg(up.y);
            up.z = Math.RadToDeg(up.z);

            return up.Normalized();
        }

        public static Vector3 CalculateForward(ref Transform3D t3d)
        {
            Vector3 rotrad;
            rotrad.x = Math.DegToRad(t3d.Rotation.x);
            rotrad.y = Math.DegToRad(t3d.Rotation.y);
            rotrad.z = Math.DegToRad(t3d.Rotation.z);

            Vector3 forward;
            forward.x = Cos(rotrad.x) * Sin(rotrad.y);
            forward.y = -Sin(rotrad.x);
            forward.z = Cos(rotrad.x) * Cos(rotrad.y);

            forward.x = Math.RadToDeg(forward.x);
            forward.y = Math.RadToDeg(forward.y);
            forward.z = Math.RadToDeg(forward.z);

            return forward.Normalized();
        }
        public static Vector3 CalculateRight(ref Transform3D t3d)
        {
            Vector3 rotrad;
            rotrad.x = Math.DegToRad(t3d.Rotation.x);
            rotrad.y = Math.DegToRad(t3d.Rotation.y);
            rotrad.z = Math.DegToRad(t3d.Rotation.z);

            Vector3 right;
            right.x = Cos(rotrad.z);
            right.y = 0f;
            right.z = -Sin(rotrad.z);

            right.x = Math.RadToDeg(right.x);
            right.y = Math.RadToDeg(right.y);
            right.z = Math.RadToDeg(right.z);

            return right.Normalized();
        }
        public static void Translate(ref Transform3D transform, Vector3 pos)
        {
            // Translate the transform based on the pos vector and the rotation of the object
            transform.LocalPosition +=
            CalculateForward(ref transform) * pos.z + CalculateRight(ref transform) * pos.x + CalculateUp(ref transform) * pos.y;
        }
        public static Vector3 ToEuler(Quaternion q)
        {
            Vector3 angles;

            // roll (x-axis rotation)
            double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
            double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
            angles.y = (float)Math.Atan2(sinr_cosp, cosr_cosp);

            // pitch (y-axis rotation)
            double sinp = Math.Sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
            double cosp = Math.Sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
            angles.x = (float)(2 * Math.Atan2(sinp, cosp) - System.Math.PI / 2);

            // yaw (z-axis rotation)
            double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
            double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
            angles.z = (float)Math.Atan2(siny_cosp, cosy_cosp);

            return angles;
        }
        #endregion

        public static float PI = 3.14159265358979323846f;

        public static float DegToRad(float deg)
        {
            return deg * Math.PI / 180.0f;
        }

        public static float RadToDeg(float rad)
        {
            return rad * 180.0f / Math.PI;
        }
    }
    public struct Quaternion
    {
        public float x, y, z, w;
        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        public Quaternion(float pitch, float yaw, float roll)
        {
            float cr = (float)Math.Cos(roll * 0.5);
            float sr = (float)Math.Sin(roll * 0.5);
            float cp = (float)Math.Cos(pitch * 0.5);
            float sp = (float)Math.Sin(pitch * 0.5);
            float cy = (float)Math.Cos(yaw * 0.5);
            float sy = (float)Math.Sin(yaw * 0.5);

            w = cr * cp * cy + sr * sp * sy;
            x = sr * cp * cy - cr * sp * sy;
            y = cr * sp * cy + sr * cp * sy;
            z = cr * cp * sy - sr * sp * cy;
        }
        public Quaternion(Vector3 euler)
        {
            float cr = (float)Math.Cos(euler.y * 0.5);
            float sr = (float)Math.Sin(euler.y * 0.5);
            float cp = (float)Math.Cos(euler.x * 0.5);
            float sp = (float)Math.Sin(euler.x * 0.5);
            float cy = (float)Math.Cos(euler.z * 0.5);
            float sy = (float)Math.Sin(euler.z * 0.5);

            w = cr * cp * cy + sr * sp * sy;
            x = sr * cp * cy - cr * sp * sy;
            y = cr * sp * cy + sr * cp * sy;
            z = cr * cp * sy - sr * sp * cy;
        }
        //Bullshit from ChatGPT
        public static Vector3 operator *(Quaternion rotation, Vector3 point)
        {
            float num = rotation.x * 2f;
            float num2 = rotation.y * 2f;
            float num3 = rotation.z * 2f;
            float num4 = rotation.x * num;
            float num5 = rotation.y * num2;
            float num6 = rotation.z * num3;
            float num7 = rotation.x * num2;
            float num8 = rotation.x * num3;
            float num9 = rotation.y * num3;
            float num10 = rotation.w * num;
            float num11 = rotation.w * num2;
            float num12 = rotation.w * num3;
            Vector3 result;
            result.x = (1f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
            result.z = (num7 + num12) * point.x + (1f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
            result.y = (num8 - num11) * point.x + (num9 + num10) * point.y + (1f - (num4 + num5)) * point.z;
            return result;
        }

    }

    public struct Vector4
    {
        public float x, y, z, w;

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
    }
    public struct Vector3
    {
        public float x, y, z;
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator *(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
        }
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }
        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }
        public static Vector3 operator *(Vector3 a, float k)
        {
            return new Vector3(a.x * k, a.y * k, a.z * k);
        }
        public static Vector3 operator-(Vector3 v)
        {
            return new Vector3(-v.x, -v.y, -v.z);
        }

        public float Magnitude()
        {
            return (float)System.Math.Sqrt(x * x + y * y + z * z);
        }

        public Vector3 Normalized()
        {
            float mag = Magnitude();

            return new Vector3(x / mag, y / mag, z / mag);
        }

        public void Normalize()
        {
            float mag = Magnitude();

            x /= mag;
            y /= mag;
            z /= mag;
        }
    }
    public struct Vector2
    {
        public float x, y;

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }

    
}
