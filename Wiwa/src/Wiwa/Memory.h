#pragma once
#pragma warning(disable : 4595)
#pragma warning(disable : 6386)
#include <memory>

struct AllocationMetrics
{
	uint32_t allocations = 0;

	size_t totalAllocated = 0;
	size_t totalFreed = 0;

	AllocationMetrics() = default;

	size_t CurrentUsage() { return totalAllocated - totalFreed; }
};

inline static AllocationMetrics s_AllocationMetrics;

inline void* operator new(size_t size)
{
	s_AllocationMetrics.totalAllocated += size;
	s_AllocationMetrics.allocations++;
	return malloc(size);
}
inline void operator delete(void* ptr, size_t size)
{
	s_AllocationMetrics.totalFreed += size;
	s_AllocationMetrics.allocations++;
	free(ptr);
}