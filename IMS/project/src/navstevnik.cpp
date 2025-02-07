#include <vector>
#include <iostream>

#include "config.hpp"
#include "navstevnik.hpp"
#include "otevreno.hpp"
#include "prijmyZeStanku.hpp"
#include "prijmyZeVstupu.hpp"
#include "atrakce3Operator.hpp"

Queue TurniketyQueue("Rada na turnikety");
Store Turnikety("Turnikety", 5);

Queue ToaletyQueue("Rada pred toaletami");
Store Toalety("Toalety", 5);

Queue StankyQueue("Rada pred stanky");
Store Stanky("Prodejni stanky", 5);

Queue Atrakce1Queue("Rada pred atrakci1");
Facility Atrakce1("Atrakce1 pro 1");

Queue Atrakce2Queue("Rada pred atrakci2");
Facility Atrakce2("Atrakce2 pro 1");

Queue Atrakce3Queue("Rada pred atrakci3");
Store Atrakce3("Atrakce3 pro " + ATRAKCE3_KAPACITA, ATRAKCE3_KAPACITA);

void Navstevnik::Behavior()
{
    misto = "Pred turnikety";
    prichod = Time;
    if (Turnikety.Full())
    {
        TurniketyQueue.Insert(this);
        this->Passivate();
        if (!jeOtevreno)
        {
            opousti();
            return;
        }
    }

    #ifdef UBRAT_ZAMESTNANCE_BEHEM_DESTE
        if (jeDest)
        {
            Enter(Turnikety, 5);
        }
        else
        {
            Enter(Turnikety, 1);
        }
    #else
        Enter(Turnikety, 1);
    #endif

    Wait(Exponential(5));

    #ifdef UBRAT_ZAMESTNANCE_BEHEM_DESTE
        if (jeDest)
        {
            Leave(Turnikety, 5);
        }
        else
        {
            Leave(Turnikety, 1);
        }
    #else
        Leave(Turnikety, 1);
    #endif

    if (!jeOtevreno)
    {
        opousti();
        return;
    }

    if (!TurniketyQueue.Empty())
    {
        (TurniketyQueue.GetFirst())->Activate();
    }

    if (jeOtevreno)
    {
        (new PrijmyZeVstupu)->Activate();
    }
    vDisneyLandu();
}

void Navstevnik::vDisneyLandu()
{
    misto = "DisneyLand";
    double choice = Random();
    if (choice <= 0.05) // 5 %
    {
        toaleta();
    }
    else if (choice <= 0.6) // 55 %
    {
        stanky();
    }
    else // 40 %
    {
        vyberAtrakce();
    }
}

void Navstevnik::toaleta()
{
    misto = "Toaleta";
    if (Toalety.Full())
    {
        ToaletyQueue.Insert(this);
        this->Passivate();
        if (!jeOtevreno)
        {
            opousti();
            return;
        }
    }

    Enter(Toalety, 1);
    double casNaToalete = Normal(5, 1);
    casNaToalete = casNaToalete < 0 ? 0 : casNaToalete;
    casNaToalete = casNaToalete > 20 ? 20 : casNaToalete;
    Wait(casNaToalete);
    Leave(Toalety, 1);

    if (!ToaletyQueue.Empty())
    {
        (ToaletyQueue.GetFirst())->Activate();
    }

    jeStaleOtevreno();
}

void Navstevnik::stanky()
{
    misto = "Stanky";
    if (Stanky.Full())
    {
        StankyQueue.Insert(this);
        this->Passivate();
        if (!jeOtevreno)
        {
            opousti();
            return;
        }
    }

    Enter(Stanky, 1);
    double casUStanku = Normal(5, 1);
    casUStanku = casUStanku < 0 ? 0 : casUStanku;
    casUStanku = casUStanku > 20 ? 20 : casUStanku;
    Wait(casUStanku);
    Leave(Stanky, 1);

    if (!StankyQueue.Empty())
    {
        (StankyQueue.GetFirst())->Activate();
    }

    (new PrijmyZeStanku)->Activate();
    jeStaleOtevreno();
}

void Navstevnik::vyberAtrakce()
{
    double choice = Random();
    if (choice <= 0.33) // 33 %
    {
        predAtrakci1();
    }
    else if (choice <= 0.66) // 33 %
    {
        predAtrakci2();
    }
    else // 34 %
    {
        predAtrakci3();
    }
}

void Navstevnik::predAtrakci1()
{
    misto = "Atrakce1";
    if (Atrakce1.Busy())
    {
        Atrakce1Queue.Insert(this);
        this->Passivate();
        if (!jeOtevreno)
        {
            opousti();
            return;
        }
    }

    Atrakce1.Seize(this);
    Wait(5);
    Atrakce1.Release(this);

    if (!Atrakce1Queue.Empty())
    {
        (Atrakce1Queue.GetFirst())->Activate();
    }

    jeStaleOtevreno();
}

void Navstevnik::predAtrakci2()
{
    misto = "Atrakce2";
    Wait(0.01);
    if (Atrakce2.Busy())
    {
        Atrakce2Queue.Insert(this);
        this->Passivate();
        if (!jeOtevreno)
        {
            opousti();
            return;
        }
    }

    Atrakce2.Seize(this);
    Wait(10);
    Atrakce2.Release(this);

    if (!Atrakce2Queue.Empty())
    {
        (Atrakce2Queue.GetFirst())->Activate();
    }

    jeStaleOtevreno();
}

void Navstevnik::predAtrakci3()
{
    misto = "Atrakce3";
    Atrakce3Queue.Insert(this);
    if (Atrakce3Queue.Length() == ATRAKCE3_KAPACITA)
    {
        (new Atrakce3Operator)->Activate();
    }
    this->Passivate();
    if (!jeOtevreno)
    {
        opousti();
        return;
    }
    Enter(Atrakce3, 1);
    Wait(10);
    Leave(Atrakce3, 1);

    jeStaleOtevreno();
}

void Navstevnik::jeStaleOtevreno()
{
    if (jeOtevreno)
    {
        muzeOdejit();
    }
    else
    {
        opousti();
    }
}

void Navstevnik::muzeOdejit()
{
    double choice = Random();
    if (choice <= 0.1) // 10 %
    {
        opousti();
    }
    else // 90 %
    {
        vDisneyLandu();
    }
}

void Navstevnik::opousti()
{
    #ifdef DEBUG
        // std::cerr << "\nMISTO: " << misto << std::endl;
        // Print("TIME: %.2f, %.2f, %.2f\n", prichod, Time, Time - prichod);
        navstevnost(Time - prichod);
    #endif
    this->Cancel();
}

void Navstevnik::vyprazdneniPoUzavreni()
{
    if (!jeOtevreno)
    {
        while (!TurniketyQueue.Empty())
        {
            (TurniketyQueue.GetFirst())->Activate();
        }
        while (!ToaletyQueue.Empty())
        {
            (ToaletyQueue.GetFirst())->Activate();
        }
        while (!StankyQueue.Empty())
        {
            (StankyQueue.GetFirst())->Activate();
        }
        while (!Atrakce1Queue.Empty())
        {
            (Atrakce1Queue.GetFirst())->Activate();
        }
        while (!Atrakce2Queue.Empty())
        {
            (Atrakce2Queue.GetFirst())->Activate();
        }
        while (!Atrakce3Queue.Empty())
        {
            (Atrakce3Queue.GetFirst())->Activate();
        }
    }
}
