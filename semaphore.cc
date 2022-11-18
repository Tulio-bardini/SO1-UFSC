#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"
#include "semaphore.h"

__BEGIN_API

void Semaphore::p() {

}

void Semaphore::v() {

}

int finc(volatile int & number) {
    int add;
    asm("lock#xadd %%eax, %%ebx;" : "=a" (add): "a" (number), "b" (1));
    return add;
}

int fdec(volatile int & number) {
    int sub;
    asm("lock#xadd %%eax, %%ebx;" : "=a" (sub): "a" (number), "b" (-1));
    return sub;
}

void sleep() {

}

void wakeup() {

}

void wakeup_all() {

}

__END_API