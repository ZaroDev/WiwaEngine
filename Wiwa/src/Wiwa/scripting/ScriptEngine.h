#pragma once

namespace Wiwa {
	class ScriptEngine
	{
	public:
		static void Init();
		static void ShutDown();
	private:
		static void InitMono();
		static void ShutDownMono();
	};
}