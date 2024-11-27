#ifndef OTEVRENO_HPP
#define OTEVRENO_HPP

#include <simlib.h>

#include "generatory.hpp"
#include "navstevnik.hpp"

extern bool jeOtevreno;
extern double vydaje;

class Otevreno : public Process
{
    void Behavior()
    {
        while (true)
        {
            Wait(6 * 60);
            jeOtevreno = true;
            double choice = Uniform(0, 1);
            Event *generator = nullptr;
            if (choice <= 0.3) // 30 %
            {
                generator = new Generator60();
            }
            else // 70 %
            {
                generator = new Generator5();
            }
            generator->Activate();

            Wait(16 * 60);

            jeOtevreno = false;
            Navstevnik::vyprazdneniPoUzavreni();
            vydaje += 500 * 16 * 9 + 100000;

            generator->Passivate();
            Wait(2 * 60);
        }
    }
};

#endif // OTEVRENO_HPP