#pragma once
#pragma warning(disable : 4251)

#include "Empathy/Core.h"

#include <chrono>
#include <iostream>

namespace EM {

	class EM_API Timer
	{
		std::chrono::time_point<std::chrono::steady_clock> start, end;

		std::chrono::duration<float> duration;

	public:
		Timer();
		~Timer();
	};
}
