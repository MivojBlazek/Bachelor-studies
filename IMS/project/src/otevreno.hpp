#ifndef OTEVRENO_HPP
#define OTEVRENO_HPP

#include <simlib.h>
#include <cmath>

#include "config.hpp"
#include "generator.hpp"
#include "navstevnik.hpp"

extern bool jeOtevreno;
extern bool jeDest;
extern double vydaje;
extern double prijmy;
extern double prijmyVDen;
extern Histogram navstevnost;

class Otevreno : public Process
{
    void Behavior()
    {
        for (int i = 1; true; i++)
        {
            double vydajeVDen = 0;
            prijmyVDen = 0;

            navstevnost.Clear();

            Wait(CAS_OTEVRENI * 60);

            jeOtevreno = true;
            jeDest = false;
            double choice = Random();
            Generator *generator = nullptr;

            double stredniHodnotaPrichodu;
            if (choice <= 0.3) // 30 %
            {
                Print("DEN %d: Dest\n", i);
                jeDest = true;
                stredniHodnotaPrichodu = 7 * std::max((1.0 / 7.0), std::pow(1.00 - VLIV_REKALMY, POCET_MARKETINGU));
            }
            else // 70 %
            {
                Print("DEN %d: Sucho\n", i);
                stredniHodnotaPrichodu = 2.5 * std::max((1.0 / 2.5), std::pow(1.00 - VLIV_REKALMY, POCET_MARKETINGU));
            }
            Print("Stredni hodnota prichodu navstevniku: %.2f\n", stredniHodnotaPrichodu);
            generator = new Generator(stredniHodnotaPrichodu);
            generator->Activate();

            Wait((CAS_UZAVRENI - CAS_OTEVRENI) * 60);

            generator->Cancel();
            jeOtevreno = false;
            Navstevnik::vyprazdneniPoUzavreni();

            double marketing = std::pow((POCET_MARKETINGU) * 30, 2.3) + VYDAJE_NA_PRVNI_MARKETING * POCET_MARKETINGU;
            marketing = marketing < 0.0 ? 0.0 : marketing;
            Print("\n---%.2f---\n", marketing);
            #ifdef UBRAT_ZAMESTNANCE_BEHEM_DESTE
                if (jeDest)
                {
                    vydajeVDen += PLAT * (CAS_UZAVRENI - CAS_OTEVRENI) * POCET_ZAMESTNANCU_V_DESTI + PROVOZNI_VYDAJE + marketing;
                }
                else
                {
                    vydajeVDen += PLAT * (CAS_UZAVRENI - CAS_OTEVRENI) * POCET_ZAMESTNANCU + PROVOZNI_VYDAJE + marketing;
                }
            #else
                vydajeVDen += PLAT * (CAS_UZAVRENI - CAS_OTEVRENI) * POCET_ZAMESTNANCU + PROVOZNI_VYDAJE + marketing;
            #endif

            vydaje += vydajeVDen;
            prijmy += prijmyVDen;

            Wait((24 - CAS_UZAVRENI) * 60);

            Navstevnik::vyprazdneniPoUzavreni();
            
            #ifdef DEBUG
                navstevnost.Output();
            #endif
            
            Print("Vydeje tento den: %.2f\n", vydajeVDen);
            Print("Prijmy tento den: %.2f\n\n", prijmyVDen);
        }
    }
};

#endif // OTEVRENO_HPP