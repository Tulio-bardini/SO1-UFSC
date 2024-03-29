#include "cpu.h"
#include "debug.h"
#include <iostream>

__BEGIN_API

void CPU::Context::save()
{
    db<CPU>(TRC) << "CPU::Context::save() chamado\n";
    getcontext(&_context);
}

void CPU::Context::load()
{
    db<CPU>(TRC) << "CPU::Context::load() chamado\n";
    setcontext(&_context);
}

CPU::Context::~Context()
{
    db<CPU>(TRC) << "CPU::Context::~Context() chamado\n";
    if (_stack) {
        delete _stack;
    }
}

int CPU::switch_context(Context *from, Context *to)
{
    db<CPU>(TRC) << "CPU::Context::switch_context() chamado\n";
    return swapcontext(&from->_context, &to->_context);
}

__END_API
