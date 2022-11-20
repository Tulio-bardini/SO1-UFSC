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
    if (_poll == _size) {
        return;
    } else {
        finc(_poll);
        wakeup();
    }

}

int Semaphore::finc(volatile int & number) {
    int one = 1;
    asm("LOCK xadd %%eax, %%ebx" : "=a"(number): "a" (number), "b" (one));
    return number;
}

int Semaphore::fdec(volatile int & number) {
    int one = -1;
    asm("LOCK xadd %%eax, %%ebx" : "=a"(number): "a" (number), "b" (one));
    return number;
}

void Semaphore::sleep() {
    Thread* running = Thread::running();
    _slept.push_back(running);
    running->sleep();
}

void Semaphore::wakeup() {
    Thread* threadSleeping = _slept.front();
    _slept.pop_front();
    Thread::wakeup(threadSleeping);
}

void Semaphore::wakeup_all() {

}

Semaphore::~Semaphore() {
    
}

__END_API