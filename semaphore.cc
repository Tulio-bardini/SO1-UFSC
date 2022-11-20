#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"
#include "semaphore.h"

__BEGIN_API

void Semaphore::p() {

    if (fdec(_poll) <= 0) {
        sleep();
    }

}

void Semaphore::v() {

    // Quando todas as threads ja tiverem sido liberadas
    if (_poll == _size) {
        return;
    } 

    if (_slept.size() > 0) {
        finc(_poll);
        wakeup();
    }

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
    _slept.pop_front();
    Thread::wakeup(threadSleeping);

}

void Semaphore::wakeup_all() {

    if (_slept.size() != 0) {
        for (int i = _slept.size(); i > 0; i--) {
            wakeup();
        }
    }

}

Semaphore::~Semaphore() {
    wakeup_all();
}

__END_API