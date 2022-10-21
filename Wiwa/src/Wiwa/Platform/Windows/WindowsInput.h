#pragma once

#include "Wiwa/Input.h"

namespace Wiwa
{
	class WindowsInput : public Input 
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual void SetMousePosImpl(float x, float y) override;
		virtual void SetCursorVisibleImpl(bool visible) override;
	};
}