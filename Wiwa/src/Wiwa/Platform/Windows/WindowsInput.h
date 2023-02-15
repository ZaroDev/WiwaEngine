#pragma once

#include <Wiwa/core/Input.h>

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
		virtual float GetMouseXDeltaImpl() override;
		virtual float GetMouseYDeltaImpl() override;
		virtual void UpdateImpl() override;
		virtual void LockCursorImpl() override;
		virtual void UnlockCursorImpl() override;
	};
}