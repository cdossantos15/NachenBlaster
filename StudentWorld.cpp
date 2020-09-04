#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{}

int StudentWorld::init()
{
    //Set the # of ships destroyed to 0
    m_shipsDestroyed = 0;
    //Set the # of ships currently on screen to 0
    m_shipsOnScreen = 0;
    
    //ADD ACTORS
    addAliens();
    /*
    ActorVector.push_back(new Smallgon (VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
    ActorVector.push_back(new Smoregon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
    ActorVector.push_back(new Snagglegon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
     */
    //Add initial stars
    for (int i = 0; i < 30; i++)
        ActorVector.push_back(new Star(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), this));
    
    //Add NachenBlaster
    m_NachenBlaster = new NachenBlaster(this);
    
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
};

int StudentWorld::move()
{
    //TELL EACH ACTOR TO DO SOMETHING
    m_NachenBlaster->doSomething();
    for (int i = 0; i < ActorVector.size(); i++)
    {
        if(ActorVector[i]->isAlive())
        {
            ActorVector[i]->doSomething();
        
            //CHECK IF NACHENBLASTER HAS DIED
            if(!m_NachenBlaster->isAlive())
                return GWSTATUS_PLAYER_DIED;
            //LEVEL COMPLETED?
            if (m_shipsDestroyed == 6+(4*getLevel()))
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    //ADD ACTORS THAT NEED TO BE ADDED
    
    //Random Stars
    if (rand() % 15 < 1)
    {
        ActorVector.push_back(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1),this));
    }
    //Alien Ships
    addAliens();
    
    //REMOVE DEAD ACTORS
    removeDeadActors();
    
    //UPDATE GAME STATUS LINE
    
    
    //CONTINUE THE GAME
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator p = ActorVector.begin(); p != ActorVector.end();)
    {
            delete *p;
            p = ActorVector.erase(p);
    }
    if (m_NachenBlaster != nullptr)
    {
    delete m_NachenBlaster;
    m_NachenBlaster = nullptr;
    }
}

void StudentWorld::addAliens()
{
    int level = getLevel();
    //Determine how many ships have been destroyed.
    int D = m_shipsDestroyed;   //Ships destroyed by player during level so far.
    int T = 6+(4*level);        //Total ships that need to be destroyed
    int R = T - D;              //Remaining ships that need to be destroyed
    int M = 4 + (0.5*level);    //Maximum number of ships that should be on screen
    
    if (m_shipsOnScreen < min(M, R))
    {
        int S1 = 60;
        int S2 = 20+level*5;
        int S3 = 5+level*10;
        int S = S1 + S2 + S3;
        //add an alien
        bool Smallgon = ((rand() % S) < S1);
        bool Smoregon = ((rand() % S) < S2);
        bool Snagglegon = ((rand() % S) < S3);
        if (Smallgon)
        {
            ActorVector.push_back(new class Smallgon (VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
            m_shipsOnScreen++;
            return;
        }
        if (Smoregon)
        {
            ActorVector.push_back(new class Smoregon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
            m_shipsOnScreen++;
            return;
        }
        if (Snagglegon)
        {
            ActorVector.push_back(new class Snagglegon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
            m_shipsOnScreen++;
            return;
        }
    }
    
    return;
}


void StudentWorld::removeDeadActors()
{
    for (vector<Actor*>::iterator p = ActorVector.begin(); p != ActorVector.end();)
    {
        if (!(*p)->isAlive())
        {
            delete *p;
            p = ActorVector.erase(p);
        }
        else
            p++;
    }
}

void StudentWorld::addDestroyedShip()
{
    m_shipsDestroyed++;
}

void StudentWorld::removeShipOnScreen()
{
    m_shipsOnScreen--;
}

NachenBlaster* StudentWorld::getNachenBlaster() const
{
    return m_NachenBlaster;
}

