#include "thread.h"
#include "cpu.h"
#include "main_class.h"
#include "debug.h"
#include <iostream>

__BEGIN_API

int Thread::next_id = 0;
Thread *Thread::_running = 0;


int Thread::switch_context(Thread * prev, Thread * next)
{
    db<Thread>(TRC) << "Thread::switch_context() chamado\n";
    db<Thread>(INF) << "Thread switch: " << prev->_id << " -> " << next->_id << "\n";
    _running = next;
    return CPU::switch_context(prev->_context, next->_context);
}

void Thread::thread_exit (int exit_code)
{
    db<Thread>(TRC) << "Thread::thread_exit() chamado\n";
    db<Thread>(INF) << "Thread exited: " << _id << "\n";
    next_id -= 1;
    delete _context;
}

int Thread::id()
{
    db<Thread>(TRC) << "Thread::id() chamado\n";
    return _id;
}

__END_API
