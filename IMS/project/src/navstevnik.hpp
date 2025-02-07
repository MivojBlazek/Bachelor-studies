#ifndef NAVSTEVNIK_HPP
#define NAVSTEVNIK_HPP

#include <simlib.h>
#include <string>

extern Queue TurniketyQueue;
extern Store Turnikety;
extern Queue ToaletyQueue;
extern Store Toalety;
extern Queue StankyQueue;
extern Store Stanky;
extern Queue Atrakce1Queue;
extern Facility Atrakce1;
extern Queue Atrakce2Queue;
extern Facility Atrakce2;
extern Queue Atrakce3Queue;
extern Store Atrakce3;

class Navstevnik : public Process
{
public:
    double prichod;
    std::string misto;

    void Behavior();
    void vDisneyLandu();
    void toaleta();
    void stanky();
    void vyberAtrakce();

    void predAtrakci1();
    void predAtrakci2();
    void predAtrakci3();

    void jeStaleOtevreno();
    void muzeOdejit();
    
    void opousti();

public:
    static void vyprazdneniPoUzavreni();
};

#endif // NAVSTEVNIK_HPP