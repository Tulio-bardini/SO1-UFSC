#include "thread.h"
#include "cpu.h"
#include "main_class.h"
#include <iostream>

__BEGIN_API

int Thread::next_id = 0;
Thread *Thread::_running = 0;


int Thread::switch_context(Thread * prev, Thread * next)
{
    CPU::switch_context(prev->_context, next->_context);
    _running = next;
    return 0;
}

void Thread::thread_exit (int exit_code)
{
    delete _context;
}

int Thread::id()
{
    return _id;
}

__END_API
