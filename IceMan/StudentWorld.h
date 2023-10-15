#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//class IceMan;
//class Ice;

//structer to use as a pointer to the 2D-Array of ice
struct IceArray { Ice* arr[60][60]; };

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir),
		currentTick(0)
	{
	}

	virtual int init()
	{
		currentTick = 0;
		nextProtester = 1;
		player = new IceMan(this);
		setUpIce();
		setUpActors();
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		++currentTick;
		UpdateDisplayText();
		addNewActors();

		player->doSomething();
		for (auto it = actors.begin(); it != actors.end(); ++it)
		{
			if ((*it)->isAlive())
			{
				(*it)->doSomething();
				if (player->isAlive() == false)
					return GWSTATUS_PLAYER_DIED;
				if (RemainingOil <= 0)
				{
					playSound(SOUND_FINISHED_LEVEL);
					return GWSTATUS_FINISHED_LEVEL;
				}
			}
		}

		removeDeadObjects();

		if (player->isAlive() == false)
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		if (RemainingOil <= 0)
		{
			playSound(SOUND_FINISHED_LEVEL);
			return GWSTATUS_FINISHED_LEVEL;
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		delete player;
		deleteIce();
		deleteActors();
	}
	void UpdateDisplayText();
	void decOil();
	IceArray* getIce();
	IceMan* getPlayer();
	int getCurrentTick() { return currentTick; };
	std::vector<Actor*>* GetActors() { return &actors; };
private:
	IceMan* player;
	IceArray ice;

	std::vector<Actor*> actors;
	int RemainingOil;
	
	int currentTick;
	int nextProtester;
	void setUpIce();
	void deleteIce();
	void setUpActors();
	void deleteActors();
	void removeDeadObjects();
	void addNewActors();
	bool inRadius(int r, Actor* act, int x, int y);
};

#endif // STUDENTWORLD_H_
