namespace Wiwa
{
    public class Resources
    {
        public static ulong LoadModel(string str)
        {
            return InternalCalls.LoadResourceModel(str);
        }
    }
}