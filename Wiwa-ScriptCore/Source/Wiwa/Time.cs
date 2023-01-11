namespace Wiwa
{
    public class Time
    {
        public static float DeltaTime()
        {
            return InternalCalls.GetDeltaTimeIntr();
        }
        public static float GetTime()
        {
            return InternalCalls.GetTimeIntr();
        }
    }
}
