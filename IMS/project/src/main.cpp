#include <iostream>
#include <simlib.h>

#include "otevreno.hpp"

bool jeOtevreno = false;
double vydaje = 0;

int main()
{
    Init(0, 60 * 24 * 7);
    (new Otevreno)->Activate();
    Run();
    return 0;
}
