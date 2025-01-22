#pragma once

#include "types.h"

bool cpuHasMSR(void);

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi);

void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi);