#include <wipch.h>
#include "AllocationMetrics.h"

namespace Wiwa {
	// Define allocation metrics
	size_t AllocationMetrics::allocation_count = 0;
	size_t AllocationMetrics::bytes_allocated = 0;
}