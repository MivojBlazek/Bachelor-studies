#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <simlib.h>
#include "navstevnik.hpp"

class Generator : public Event
{
private:
    int expMinutes;

public:
    Generator(int _expMinutes) : expMinutes(_expMinutes)
    {
        //
    }

    void Behavior()
    {
        (new Navstevnik)->Activate();
        Activate(Time + Exponential(expMinutes));
    }
};

#endif // GENERATOR_HPP