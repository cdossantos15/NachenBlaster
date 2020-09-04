#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <algorithm>
#include <vector>
#include <random>

class Actor;
class NachenBlaster;
class Alien;
class Smallgon;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void addAliens();
    void removeDeadActors();
    void addDestroyedShip();
    void removeShipOnScreen();
    NachenBlaster* getNachenBlaster() const;
    
private:
    std::vector<Actor*> ActorVector;
    NachenBlaster* m_NachenBlaster;
    int m_shipsDestroyed;
    int m_shipsOnScreen;
};

#endif // STUDENTWORLD_H_
