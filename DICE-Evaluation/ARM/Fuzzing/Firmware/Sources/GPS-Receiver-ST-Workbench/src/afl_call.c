#include <stdint.h>

int noHyperCall = 0; // don't make hypercalls

__attribute__ ((naked)) uint32_t aflCall(__attribute__ ((unused)) uint32_t a0, __attribute__ ((unused)) uint32_t a1, __attribute__ ((unused)) int32_t a2) {
    /*
     * In qemu, svc $0x3f is intercepted, w/o really executing it
     * On real dev, it is executed, what's the expected result?
     * hypercall can be truned off by set noHyperCall to 0
     *
     * Why use bx rather than b?
     */
    __asm__ __volatile__ ("svc $0x3f\n\t"
                          "bx %lr\n\t");
}

int startForkserver(int ticks) {
// @param ticks: either enable or disable the CPUs timer in each forked child
    if(noHyperCall)
        return 0;
    return aflCall(1, ticks, 0);
}

/*
int doneWork(int val) {
    if(noHyperCall)
        return 0;
    return aflCall(4, (uint32_t)val, 0);
}
*/
