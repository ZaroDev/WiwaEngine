#pragma once

#include <Wiwa/Core.h>
#include <chrono>

namespace Wiwa
{

	class WI_API Time
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_GameClock;
		std::chrono::time_point<std::chrono::steady_clock> m_RealTimeClock;
		uint32_t m_FrameCount = 0;
		std::chrono::duration<float> m_Time;
		int m_TimeScale = 1;
		std::chrono::duration<float> m_DeltaTime;
		std::chrono::time_point<std::chrono::steady_clock> m_RealLastTime;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
		std::chrono::duration<float> m_RealTimeSinceStartup;
		std::chrono::duration<float> m_RealTimeDeltaTime;
		bool m_IsPlaying = false;
	public:
		Time();
		~Time();

		void Play();
		void Update();
		void Stop();

		static inline Time& Get() { return *s_Instance; }

		inline uint32_t GetFrameCount() { return m_FrameCount; }
		inline float GetTime() { return m_Time.count(); }
		inline int GetTimeScale() { return m_TimeScale; }
		void SetTimeScale(const int value);
		inline float GetDeltaTime() { return m_DeltaTime.count() * 1000.0f; }
		inline float GetRealTimeSinceStartup() { return m_RealTimeSinceStartup.count(); }
		inline float GetRealDeltaTime() { return m_RealTimeDeltaTime.count() * 1000.0f; }
		inline bool IsPlaying() { return m_IsPlaying; }
	private:
		static Time* s_Instance;
	};
}
