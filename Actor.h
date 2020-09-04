#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <stdio.h>
#include <stdlib.h>


class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

///////BASE CLASS////////////////////////
class Actor: public GraphObject
{
    //TODO: Can it be attacked?
public:
    //Constructor//
    //Each Actor has a pointer to StudentWorld;
    Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* pSW);
    //Destructor//
    virtual ~Actor();
    
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const;
    bool isAlive() const {return m_isAlive;};
    virtual void setDead() {m_isAlive = false;};
    void removeShipOnScreen();
    void addDestroyedShip();
    bool haveWeCollided(Actor* other) const;

private:
    bool m_isAlive;
    StudentWorld *m_world; //Access StudentWorld
    
};


////////// SUBCLASSES of ACTOR ////////////////////////

////////
//STAR//
////////
class Star: public Actor
{
public:
    Star(double startX, double startY, StudentWorld* pSW);
    virtual ~Star();
    virtual void doSomething();

private:
};

/////////////////
//NACHENBLASTER//
/////////////////
class NachenBlaster: public Actor
{
public:
    NachenBlaster(StudentWorld* pSW);
    virtual ~NachenBlaster();
    virtual void doSomething();
    void sufferDamage(int amount);
private:
    int m_hitPoints;
    int m_energyPoints;
};


//////////
//ALIENS//
//////////
class Alien: public Actor
{
public:
    Alien(int imageID, double startX, double startY, StudentWorld* pSW, unsigned int hitpoints, double travelSpeed, int flightPlan);
    void doSomething();
    virtual void checkFlightPlan(); //for Smoregon and Smallgon; different one for Snagglegon
    //~Alien();
private:
    int m_hitPoints;
    double m_travelSpeed;
    int m_flightPlanLength; //Smoregon and Smallgon only
    int m_travel_direction;
};

//Smallgon//
class Smallgon: public Alien
{
public:
    Smallgon(double startX, double startY, StudentWorld* pSW);
private:
};

//Smoregon//
class Smoregon: public Alien
{
public:
    Smoregon(double startX, double startY, StudentWorld* pSW);
private:
};

//Snagglegon//
class Snagglegon: public Alien
{
public:
    Snagglegon(double startX, double startY, StudentWorld* pSW);
    //virtual void checkFlightPlan();
private:
    int travelDirection;
    //
};

///////////////
//PROJECTILES//
///////////////

class Projectile: public Actor
{
public:
    Projectile(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* pSW);
private:
};










#endif // ACTOR_H_
