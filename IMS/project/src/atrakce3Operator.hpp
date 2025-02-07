#ifndef ATRAKCE3_OPERATOR_HPP
#define ATRAKCE3_OPERATOR_HPP

#include <simlib.h>

#include "config.hpp"
#include "navstevnik.hpp"

class Atrakce3Operator : public Process
{
    void Behavior()
    {
        while (!Atrakce3.Empty())
        {
            //
        }
        for (int i = 0; i < ATRAKCE3_KAPACITA; i++)
        {
            (Atrakce3Queue.GetFirst())->Activate();
        }
    }
};

#endif // ATRAKCE3_OPERATOR_HPP