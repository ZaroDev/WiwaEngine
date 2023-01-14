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
        public static int GetMouseX()
        {
            return InternalCalls.GetMouseXIntr();
        }
        public static int GetMouseY()
        {
            return InternalCalls.GetMouseYIntr();
        }
    }
}
