#ifndef GENERATORY_HPP
#define GENERATORY_HPP

#include <simlib.h>
#include "navstevnik.hpp"

class Generator5 : public Event
{
    void Behavior()
    {
        (new Navstevnik)->Activate();
        Activate(Time + Exponential(5));
    }
};

class Generator60 : public Event
{
    void Behavior()
    {
        (new Navstevnik)->Activate();
        Activate(Time + Exponential(60));
    }
};

#endif // GENERATORY_HPP