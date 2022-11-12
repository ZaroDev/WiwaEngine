using System;

namespace Wiwa
{
    public class Main
    {
        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintLine()
        {
            Console.WriteLine("Hello World from C#");
        }
        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says {value}");
        }
        public void PrintCustomLine(string message)
        {
            Console.WriteLine($"C# says {message}");
        }
    }
}