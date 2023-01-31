#pragma once
#include <cstdlib>
#include <Wiwa/core/Core.h>
#pragma warning(disable : 4595)
namespace Wiwa {
	// Allocation metrics
	struct WI_API AllocationMetrics {
		static size_t allocation_count;
		static size_t bytes_allocated;
	};
}

// Allocation overloads
inline void* operator new(size_t size) {
	if (size <= 0) return NULL;

	Wiwa::AllocationMetrics::allocation_count++;
	Wiwa::AllocationMetrics::bytes_allocated += size;
	if (Wiwa::AllocationMetrics::bytes_allocated > FLT_MAX)
		Wiwa::AllocationMetrics::bytes_allocated = FLT_MAX;
	return malloc(size);
}

inline void operator delete(void* memory, size_t size) {
	Wiwa::AllocationMetrics::allocation_count--;
	Wiwa::AllocationMetrics::bytes_allocated -= size;

	free(memory);
}