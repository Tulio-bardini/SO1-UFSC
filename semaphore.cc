#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"
#include "semaphore.h"
#include "cpu.h"

__BEGIN_API

void Semaphore::p() {

    if (fdec(_poll) <= 0) {
        sleep();
    }

}

void Semaphore::v() {

    // Quando todas as threads ja tiverem sido liberadas
    if (_poll >= _size) {
        return;
    }

    if (finc(_poll) < 0) {
        wakeup();
    }

}

int Semaphore::finc(volatile int & number) {
    return CPU::finc(number);
}

int Semaphore::fdec(volatile int & number) {
    return CPU::fdec(number);
}

void Semaphore::sleep() {
    Thread* running = Thread::running();
    _sleeping.push_back(running);
    running->sleep();
}

void Semaphore::wakeup() {

    Thread* threadSleeping = _sleeping.front();
    _sleeping.pop_front();

    Thread::wakeup(threadSleeping);

}

void Semaphore::wakeup_all() {

    if (_sleeping.size() != 0) {
        for (int i = _sleeping.size(); i > 0; i--) {
            wakeup();
        }
    }

}

Semaphore::~Semaphore() {
    wakeup_all();
}

__END_API