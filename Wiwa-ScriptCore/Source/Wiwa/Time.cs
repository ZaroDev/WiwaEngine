namespace Wiwa
{
    public class Time
    {
        //Time returned in seconds
        public static float DeltaTime()
        {
            return InternalCalls.GetDeltaTimeIntr() / 1000;
        }
        public static float DeltaTimeMS()
        {
            return InternalCalls.GetDeltaTimeIntr();
        }
        public static float GetTime()
        {
            return InternalCalls.GetTimeIntr();
        }
    }
}
