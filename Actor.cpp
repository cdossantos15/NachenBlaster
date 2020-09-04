//NACHENBLASTER 2

#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//CONSTANTS AND FORMULA FUNCTIONS
const int DUE_LEFT = 0;
const int UP_LEFT = 1;
const int DOWN_LEFT = 2;

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

double euclidian_dist(int x1, int y1, int x2, int y2)
{
    int d1 = x1-x2;
    int d2 = y1-y2;
    double a = d1*d1;
    double b = d2*d2;
    double result = sqrt((a+b));
    return result;
}


/////////////////////////
///////// ACTOR /////////
/////////////////////////

Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* pSW): GraphObject(imageID, startX, startY, dir, size,depth), m_world(pSW){ m_isAlive = true;};
//every actor is alive when it is first created

Actor:: ~Actor()
{}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}


void Actor::removeShipOnScreen()
{
    m_world->removeShipOnScreen();
}

void Actor::addDestroyedShip()
{
    m_world->addDestroyedShip();
}

bool Actor::haveWeCollided(Actor* other) const
{
    int x1 = getX();
    int y1 = getY();
    int x2 = other->getX();
    int y2 = other->getY();
    int R1 = getRadius();
    int R2 = other->getRadius();
    
    double comparison = 0.75*(R1+R2);
    if (euclidian_dist(x1, y1, x2, y2) < comparison)
        return true;
    return false;
}

////////////////////////
///////// STAR /////////
////////////////////////

Star::Star(double startX, double startY, StudentWorld* pSW):        //Constructor
Actor(IID_STAR, startX, startY, 0, randfrom(0.05, 0.5), 3, pSW)
{}

Star::~Star(){};

void Star::doSomething()
{
    if(getX() < 0)
        setDead();
    else
    {
    int X = getX();
    int Y = getY();
    if (X >= 0)
        moveTo(X-1, Y);
    }
}

/////////////////////////////////
///////// NACHENBLASTER /////////
/////////////////////////////////

NachenBlaster::NachenBlaster(StudentWorld* pSW):     //Constructor **Need hit points and energy points
Actor(IID_NACHENBLASTER, 0, 128, 0, 1, 0, pSW)
{
    m_hitPoints = 50;
    m_energyPoints = 30;
}

NachenBlaster::~NachenBlaster(){}

void NachenBlaster::doSomething()
{
    if (!isAlive())         //Check if NachenBlaster is alive.
        return;
    int ch;
    int x = getX();
    int y = getY();
    if (getWorld()->getKey(ch))     //Check if key was pressed by player.
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                if(x-6 <= 0)
                    moveTo(0, y);
                else
                    moveTo(x-6, y);
                break;
            case KEY_PRESS_RIGHT:
                if(x+6 >= VIEW_WIDTH)
                    moveTo(VIEW_WIDTH, y);
                else
                    moveTo(x+6, y);
                break;
            case KEY_PRESS_UP:
                if(y+6 >= VIEW_HEIGHT)
                    moveTo(x, VIEW_HEIGHT);
                else
                    moveTo(x, y+6);
                break;
            case KEY_PRESS_DOWN:
                if(y-6 <= 0)
                    moveTo(x, 0);
                else
                    moveTo(x, y-6);
            case KEY_PRESS_SPACE:
                break;                  //fires cabbages
            case KEY_PRESS_TAB:         //fires torpedoes
                break;
        }
    }
    if(m_energyPoints < 30)         //Add 1 Cabbage energy point per tick.
        m_energyPoints++;
        
}

void NachenBlaster::sufferDamage(int amount)
{
    m_hitPoints = m_hitPoints-amount;
}

/////////////////////////
///////// ALIEN /////////
/////////////////////////

Alien::Alien(int imageID, double startX, double startY, StudentWorld* pSW, unsigned int hitpoints, double travelSpeed, int flightPlan): Actor(imageID, startX, startY, 0, 1.5, 1, pSW)
{
    m_hitPoints = hitpoints;
    m_travelSpeed = travelSpeed;
    m_flightPlanLength = flightPlan;
    m_travel_direction = DOWN_LEFT;
}

void Alien::doSomething() //What all aliens do
{
    int x = getX();
    int y = getY();
    
    if(!isAlive())  //Check if Alien is alive.
       return;
    //Determine if it's flown off screen
    if (x < 0)
    {
        setDead();
        removeShipOnScreen();
        return;
    }
    
    //Check to see if it has collided with NachenBlaster
    StudentWorld* sw = getWorld();
    NachenBlaster* nB = sw->getNachenBlaster();
    if (haveWeCollided(nB))
    {
        m_hitPoints = m_hitPoints-100;
        //damage NB
        getWorld()->playSound(SOUND_DEATH);
        setDead();
        addDestroyedShip();
        removeShipOnScreen();
        
    }
    
    //Check if it needs new flight plan
    checkFlightPlan();
    
    //Possibly Fire somethings
    
    //Try to move
    if (m_travel_direction == UP_LEFT)
        moveTo(x-m_travelSpeed, y+m_travelSpeed);
    if (m_travel_direction == DOWN_LEFT)
        moveTo(x-m_travelSpeed, y-m_travelSpeed);
    if (m_travel_direction == DUE_LEFT)
        moveTo(x-m_travelSpeed, y);
    //Reduce Flight Plan by one (if applicable)
    m_flightPlanLength = m_flightPlanLength-1;
    //DIFFERENT FUNCTION FOR EACH
    
}

void Alien::checkFlightPlan() //VIRTUAL: this one is for Smallgon and Smoregon
{
    int y = getY();
    if (m_flightPlanLength != 0 && y > 0 && y < VIEW_HEIGHT-1 && m_travelSpeed == 2)
        return;
    if (m_flightPlanLength < 0 && y > 0 && y < VIEW_HEIGHT-1)
        return;
    //Change Directions and Flight Plan
    else
    {
        if (y >= VIEW_HEIGHT-1)
            m_travel_direction = DOWN_LEFT;
        else if (y <= 0)
            m_travel_direction = UP_LEFT;
        else if (m_flightPlanLength == 0 && m_travelSpeed == 2)
            m_travel_direction = randInt(0, 2);
        m_flightPlanLength = randInt(1, 32);
    }
}


///////// Smallgon //////////

Smallgon:: Smallgon(double startX, double startY, StudentWorld* pSW): Alien(IID_SMALLGON, startX, startY,pSW, 5*(1+(pSW->getLevel()-1)*0.1), 2.0, 0){}


///////// Smoregon /////////

Smoregon:: Smoregon(double startX, double startY, StudentWorld* pSW): Alien(IID_SMOREGON, startX, startY,pSW, 5*(1+(pSW->getLevel()-1)*0.1), 2.0, 0){}


///////// Snagglegon /////////

Snagglegon:: Snagglegon(double startX, double startY, StudentWorld* pSW): Alien(IID_SNAGGLEGON, startX, startY,pSW, 10*(1+(pSW->getLevel()-1)*0.1), 1.75, -5){}


///////////////////////////////
///////// PROJECTILES /////////
///////////////////////////////






