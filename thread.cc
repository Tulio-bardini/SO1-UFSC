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
    CPU::switch_context(prev->_context, next->_context);
    _running = next;
    return 0;
}

void Thread::thread_exit (int exit_code)
{
    db<Thread>(TRC) << "Thread::thread_exit() chamado\n";
    db<Thread>(INF) << "Thread exited: " << _id << "\n";
    delete _context;
}

int Thread::id()
{
    return _id;
}

__END_API
