#include "prijmyZeStanku.hpp"

double prijmy = 0;

void PrijmyZeStanku::Behavior()
{
    double choice = Uniform(0, 1);
    if (choice <= 0.35) // 35 %
    {
        prijmy += 20;
    }
    else if (choice <= 0.75) // 40 %
    {
        prijmy += 80;
    }
    else // 25 %
    {
        prijmy += 150;
    }
}