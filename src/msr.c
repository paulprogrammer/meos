#include "msr.h"
#include <cpuid.h>

const uint32_t CPUID_FLAG_MSR = 1 << 5;

bool cpuHasMSR() {
   static uint32_t a, b, c, d; // eax, ebx, ecx, edx
   __get_cpuid(1, &a, &b, &c, &d);
   return d & CPUID_FLAG_MSR;
}

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
   __asm__ volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
   __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
