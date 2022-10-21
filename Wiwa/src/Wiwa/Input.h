#pragma once

#include "Wiwa/Core.h"

namespace Wiwa
{
	class WI_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static void SetMousePos(float x, float y) { return s_Instance->SetMousePosImpl(x, y); }
		inline static void SetCursorVisible(bool visible) { return s_Instance->SetCursorVisibleImpl(visible); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual void SetMousePosImpl(float x, float y) = 0;
		virtual void SetCursorVisibleImpl(bool visible) = 0;
	private:
		static Input* s_Instance;
	};
}