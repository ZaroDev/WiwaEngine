#include "wipch.h"
#include "Time.h"

namespace Wiwa
{

	Time* Time::s_Instance = nullptr;
	Time::Time()
	{
		WI_CORE_ASSERT(!s_Instance, "Time already exists!");
		s_Instance = this;
		m_RealTimeClock = std::chrono::high_resolution_clock::now();
	}
	Time::~Time()
	{
	}
	void Time::Play()
	{
		m_IsPlaying = true;
		m_GameClock = std::chrono::high_resolution_clock::now();
		m_TimeScale = 1;
	}
	void Time::Update()
	{
		m_FrameCount++;
		m_RealTimeDeltaTime = std::chrono::high_resolution_clock::now() - m_RealLastTime;
		m_RealLastTime = std::chrono::high_resolution_clock::now();
		m_RealTimeSinceStartup = std::chrono::high_resolution_clock::now() - m_RealTimeClock;
		if (m_IsPlaying)
		{
			m_DeltaTime = (std::chrono::high_resolution_clock::now() - m_LastTime) * m_TimeScale;
			m_LastTime = std::chrono::high_resolution_clock::now();
			m_Time = std::chrono::high_resolution_clock::now() - m_GameClock;
		}

	}
	void Time::Stop()
	{
		m_IsPlaying = false;
		m_TimeScale = 0;
	}

	void Time::SetTimeScale(const int value)
	{
		m_TimeScale = value;
		CLAMP(m_TimeScale, -2, 5);
	}
	
}