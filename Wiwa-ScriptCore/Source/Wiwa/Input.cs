using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Wiwa
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keyCode)
        {
            return InternalCalls.IsKeyDownIntr(keyCode);
        }
        public static bool IsMouseKeyDown(int button)
        {
            return InternalCalls.IsMouseButtonPressedIntr(button);
        }
    }
}
