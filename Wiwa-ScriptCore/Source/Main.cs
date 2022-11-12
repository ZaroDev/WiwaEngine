using System;
using System.Runtime.CompilerServices;

namespace Wiwa
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CppFunc();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string log, int param);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param, out Vector3 result);
    }

    public class Entity
    {
        public Entity()
        {
            Console.WriteLine("Main constructor!");
            Log("This log is from C#", 12345);
            Vector3 pos = new Vector3(0.2f, 1.0f, 3.4f);
            Vector3 res = Log(ref pos);
            Console.WriteLine($"{res.x} {res.y} {res.z}");
        }

        public void PrintLine()
        {
            Console.WriteLine("Hello World from C#");
            InternalCalls.CppFunc();
        }
        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says {value}");
        }
        public void PrintCustomLine(string message)
        {
            Console.WriteLine($"C# says {message}");
        }
        public void Log(string log, int param)
        {
            InternalCalls.NativeLog(log, param);
        }
        public Vector3 Log(ref Vector3 param)
        {
            InternalCalls.NativeLogVector(ref param, out Vector3 result);
            return result;
        }
       
    }
}