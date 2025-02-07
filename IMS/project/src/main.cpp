#include <iostream>
#include <simlib.h>

#include "config.hpp"
#include "otevreno.hpp"

bool jeOtevreno = false;
bool jeDest = false;
double vydaje = 0;
double prijmy = 0;
double prijmyVDen = 0;
Histogram navstevnost("Doba navstevniku v minutach", 0, 60, CAS_UZAVRENI - CAS_OTEVRENI + 1);

int main()
{
    // SetOutput("data.txt");
    Init(0, 60 * 24 * POCET_DNI);
    (new Otevreno)->Activate();
    Run();
    Print("Celkove vydaje: %.2f za %d dni\n", vydaje, POCET_DNI);
    Print("Celkove prijmy: %.2f za %d dni\n", prijmy, POCET_DNI);
    return 0;
}
