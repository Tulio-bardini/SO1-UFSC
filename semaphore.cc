#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"
#include "semaphore.h"

__BEGIN_API

void Semaphore::p() {

    if (_poll == 0) {
        sleep();
    } else {
        fdec(_poll);
    }

}

void Semaphore::v() {

    // Quando todas as threads ja tiverem sido liberadas
    std::cout << _poll << ": ANTES\n";
    finc(_poll);
    std::cout << _poll << ": DEPOIS\n";
    wakeup();

}

int Semaphore::finc(volatile int & number) {
    int one = 1;
    __asm__ __volatile__ ( "lock ; xadd %0, %1;": "=r"(one) : "m"(number), "0" (one) : "memory");
    return number;
}

int Semaphore::fdec(volatile int & number) {
    int one = -1;
    __asm__ __volatile__ ( "lock ; xadd %0, %1;": "=r"(one) : "m"(number), "0" (one) : "memory");
    return number;
}

void Semaphore::sleep() {
    Thread* running = Thread::running();
    _slept.push_back(running);
    running->sleep();
}

void Semaphore::wakeup() {
    
    Thread* threadSleeping = _slept.front();
    _slept.remove(threadSleeping);
    fdec(_poll);
    Thread::wakeup(threadSleeping);
}

void Semaphore::wakeup_all() {

}

Semaphore::~Semaphore() {
    
}

__END_API