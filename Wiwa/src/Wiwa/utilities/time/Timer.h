#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/core/Core.h>

#include <chrono>
#include <iostream>

namespace Wiwa {
	namespace Time
	{
		class WI_API Timer
		{
			std::chrono::time_point<std::chrono::steady_clock> start, end;

			std::chrono::duration<float> duration;
		public:
			Timer();
			~Timer();
		};
	}
}
