#include "config.hpp"
#include "prijmyZeStanku.hpp"
#include "otevreno.hpp"

void PrijmyZeStanku::Behavior()
{
    double choice = Random();
    if (choice <= 0.2) // 20 %
    {
        prijmyVDen += PRIJMY_Z_NAPOJU;
    }
    else if (choice <= 0.6) // 40 %
    {
        prijmyVDen += PRIJMY_Z_JIDLA;
    }
    else // 40 %
    {
        prijmyVDen += PRIJMY_Z_SUVENYRU;
    }
}