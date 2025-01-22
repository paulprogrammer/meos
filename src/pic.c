#include <cpuid.h>
#include "io.h"

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

static inline void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
	__asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

static inline void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
	__asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

void __legacy_pic_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

/* Example: Get CPU's model number */
int __get_model(void) {
    int ebx, unused;
    __cpuid(0, unused, ebx, unused, unused);
    return ebx;
}

/* Example: Check for builtin local APIC. */
int __check_apic(void) {
    unsigned int eax, unused, edx;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}

/* Set the physical address for local APIC registers */
void cpu_set_apic_base(uintptr_t apic) {
   uint32_t edx = 0;
   uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

#ifdef __PHYSICAL_MEMORY_EXTENSION__
   edx = (apic >> 32) & 0x0f;
#endif

   cpuSetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

/**
 * Get the physical address of the APIC registers page
 * make sure you map it to virtual memory ;)
 */
uintptr_t cpu_get_apic_base() {
   uint32_t eax, edx;
   cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);

#ifdef __PHYSICAL_MEMORY_EXTENSION__
   return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
   return (eax & 0xfffff000);
#endif
}

void enable_apic() {
	__legacy_pic_disable();
    /* Section 11.4.1 of 3rd volume of Intel SDM recommends mapping the base address page as strong uncacheable for correct APIC operation. */

    /* Hardware enable the Local APIC if it wasn't enabled */
    cpu_set_apic_base(cpu_get_apic_base());

    /* Set the Spurious Interrupt Vector Register bit 8 to start receiving interrupts */
    (*(volatile uint32_t *) 0xF0) = (*(volatile uint32_t *)0xF0) | 0x100;
}
