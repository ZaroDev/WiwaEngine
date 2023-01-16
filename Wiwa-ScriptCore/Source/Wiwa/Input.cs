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
        public static float GetMouseX()
        {
            return InternalCalls.GetMouseXIntr();
        }
        public static float GetMouseY()
        {
            return InternalCalls.GetMouseYIntr();
        }
        public static float GetMouseXDelta()
        {
            return InternalCalls.GetMouseXDeltaIntr();
        }
        public static float GetMouseYDelta()
        {
            return InternalCalls.GetMouseYDeltaIntr();
        }
    }
}
