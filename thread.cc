#include "thread.h"
#include "cpu.h"
#include "main_class.h"
#include "debug.h"
#include <iostream>
#include <limits.h>
#include "list.h"

__BEGIN_API

int Thread::_next_id = 0;
Ordered_List<Thread> Thread::_ready = Ordered_List<Thread>();
Thread *Thread::_running = 0;
Thread Thread::_main = Thread(); 
Thread Thread::_dispatcher = Thread();
CPU::Context Thread::_main_context = CPU::Context();

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
    _state = FINISHING;
    _next_id -= 1;
    yield();
}

void Thread::init(void (*main)(void *)) {
    db<Thread>(TRC) << "Thread::thread_init() chamado\n";
    // Create Thread main
    new (&_main) Thread(main,(void*) "main");
    _main._link.rank(INT_MAX);

    // Create Thread dispatcher
    new (&_dispatcher) Thread(Thread::dispatcher);
    _dispatcher._link.rank(INT_MAX);

    db<Thread>(INF) << "Main Thread ID: " << Thread::_main.id() << "\n";
    db<Thread>(INF) << "Dispatcher Thread ID: " << Thread::_dispatcher.id() << "\n";

    // Switch context to Thread Main
    // Thread current_thread = Thread();
    // Thread::switch_context(&current_thread, &_main);

    CPU::Context _main_context;
    _running = &_main;
    _running->_state = RUNNING;
    _ready.remove(&_main);

    CPU::switch_context(&_main_context, _main.context());
}

void Thread::dispatcher() {
    db<Thread>(TRC) << "Thread::dispatcher() chamado\n";

    while (Thread::_next_id >= 2) {
        // Choose next thread
        
        Thread *next_thread = _ready.remove_head()->object();

        // Update Dispatcher Thread status and insert it into _ready 
        _dispatcher._state = READY;
        _dispatcher._link = Ready_Queue::Element(&_dispatcher, (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()));
        // Pq aqui precisa passar um ponteiro ao invés do próprio _link?
        _ready.insert(&_dispatcher._link);
        
        // Update _running pointer 
        _running = next_thread;

        // Update next thread status
        _running->_state = RUNNING;

        // Switch threads
        switch_context(&_dispatcher, _running);

        if (next_thread->_state == FINISHING) {
            _ready.remove(next_thread);
        }
    }

    // Update dispatcher thread status
    _dispatcher._state = FINISHING;

    // Remove dispatcher thread from ready queue
    _ready.remove(&_dispatcher);

    // Switch thread back to main
    switch_context(&_dispatcher, &_main);
}

void Thread::yield()
{

    db<Thread>(TRC) << "Thread::yield chamado\n";
    db<Thread>(TRC) << "_ready size: " << _ready.size() << "\n";
    
    if (_running->_state != FINISHING &&
        _running->_id != _main._id &&
        _running->_id != _dispatcher._id)
    {
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        _running->_link.rank(now);
    }

    db<Thread>(INF) << "Yielding Thread: " << _running->_id << "\n";

    _ready.insert(&_running->_link);
    Thread* current_thread = _running;
    _running = _ready.remove(&_dispatcher)->object();
    _running->_state = RUNNING;

    Thread::switch_context(current_thread, &_dispatcher);
}

Thread::~Thread() {
    db<Thread>(TRC) << "Thread::~Thread() chamado\n";
    db<Thread>(INF) << "Thread " << _id << " destruída\n";
    delete _context;
}

__END_API
