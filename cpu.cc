#include "cpu.h"
#include <iostream>

__BEGIN_API

void CPU::Context::save()
{
    db<Thread>(TRC) << "CPU::Context::save() chamado\n";
    getcontext(&_context);
}

void CPU::Context::load()
{
    db<Thread>(TRC) << "CPU::Context::load() chamado\n";
    setcontext(&_context);
}

CPU::Context::~Context()
{
    db<Thread>(TRC) << "CPU::Context::~Context() chamado\n";
    delete _stack;
}

void CPU::switch_context(Context *from, Context *to)
{
    db<Thread>(TRC) << "CPU::switch_context() chamado\n";
    from->save();
    to->load();
}

__END_API
