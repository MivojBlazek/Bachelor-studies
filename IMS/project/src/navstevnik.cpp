#include "navstevnik.hpp"
#include "otevreno.hpp"
#include "prijmyZeStanku.hpp"
#include "prijmyZeVstupu.hpp"

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
Facility Atrakce3("Atrakce3 pro 10");

void Navstevnik::Behavior()
{
    if (Turnikety.Full())
    {
        TurniketyQueue.Insert(this);
        if (!jeOtevreno)
        {
            this->Activate();
            return;
        }
        else
        {
            this->Passivate();
        }
    }

    Enter(Turnikety, 1);
    Wait(Exponential(5));
    Leave(Turnikety, 1);

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
    double choice = Uniform(0, 1);
    if (choice <= 0.05) // 5 %
    {
        toaleta();
    }
    else if (choice <= 0.20) // 15 %
    {
        stanky();
    }
    else // 80 %
    {
        vyberAtrakce();
    }
}

void Navstevnik::toaleta()
{
    if (Toalety.Full())
    {
        ToaletyQueue.Insert(this);
        if (!jeOtevreno)
        {
            this->Activate();
            return;
        }
        else
        {
            this->Passivate();
        }
    }

    Enter(Toalety, 1);
    Wait(Exponential(5));
    Leave(Toalety, 1);

    if (!ToaletyQueue.Empty())
    {
        (ToaletyQueue.GetFirst())->Activate();
    }

    jeStaleOtevreno();
}

void Navstevnik::stanky()
{
    if (Stanky.Full())
    {
        StankyQueue.Insert(this);
        if (!jeOtevreno)
        {
            this->Activate();
            return;
        }
        else
        {
            this->Passivate();
        }
    }

    Enter(Stanky, 1);
    Wait(Exponential(5));
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
    double choice = Uniform(0, 1);
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
    if (Atrakce1.Busy())
    {
        Atrakce1Queue.Insert(this);
        if (!jeOtevreno)
        {
            this->Activate();
            return;
        }
        else
        {
            this->Passivate();
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
    if (Atrakce2.Busy())
    {
        Atrakce2Queue.Insert(this);
        if (!jeOtevreno)
        {
            this->Activate();
            return;
        }
        else
        {
            this->Passivate();
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
    if (Atrakce3Queue.Length() < 10)
    {
        Atrakce3Queue.Insert(this);
        if (!jeOtevreno)
        {
            this->Activate();
            return;
        }
        else
        {
            this->Passivate();
        }
    }
    else
    {
        for (int i = 0; i < 10; i++)
        {
            (Atrakce3Queue.GetFirst())->Activate();
        }
    }

    Atrakce3.Seize(this);
    Wait(30);
    Atrakce3.Release(this);
    
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
    double choice = Uniform(0, 1);
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
    //
}

void Navstevnik::vyprazdneniPoUzavreni()
{
    if (!jeOtevreno)
    {
        while (!TurniketyQueue.Empty())
        {
            TurniketyQueue.Clear();
        }
        
        while (!ToaletyQueue.Empty())
        {
            ToaletyQueue.Clear();
        }

        while (!StankyQueue.Empty())
        {
            StankyQueue.Clear();
        }

        while (!Atrakce1Queue.Empty())
        {
            Atrakce1Queue.Clear();
        }

        while (!Atrakce2Queue.Empty())
        {
            Atrakce2Queue.Clear();
        }

        while (!Atrakce3Queue.Empty())
        {
            Atrakce3Queue.Clear();
        }
    }
}
