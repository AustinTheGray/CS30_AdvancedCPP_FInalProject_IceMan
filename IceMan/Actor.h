#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>
#include <queue>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;



class Actor : public GraphObject
{
public:
	Actor(int ID, int start_X, int start_Y, StudentWorld* wrld = nullptr, Direction d = right, double size = 1.0, unsigned int depth = 0)
		: GraphObject(ID, start_X, start_Y, d, size, depth),
		world(wrld),
		alive(true)
	{};
	bool inRadius(int radius, Actor* act);
	bool inRadius(int radius, Actor* act, int x, int y);
	bool moreThanRadius(int radius, Actor* act);
	bool moreThanRadius(int radius, Actor* act, int x, int y);

	virtual void doSomething() = 0;
	virtual bool isGiveUp() { return false; };
	virtual void setAnnoyed(int a, bool boulder = false) {};
	virtual void gotGold() {};

	StudentWorld* getWorld();

	bool isAlive() { return alive; };
	void setAlive(bool alv) { alive = alv; };

	int getTick();
	void setTick(int tick);

	int getHP();
	void setHP(int amount);

private:
	//never destruct this!! 
	//there wont be a leak. 
	//if you destruct it. 
	//you will get errors.
	StudentWorld* world;

	bool alive;
	int tickToReach;
	int hp;
};

class vector2D
{
public:
	vector2D() : x(0), y(0) {};
	vector2D(Actor* act) { x = act->getX(); y = act->getY(); };
	vector2D(int x, int y) : x(x), y(y) {};
	int x, y;

	~vector2D() {};
};

struct Path
{
	Path() {};
	
	vector2D arr[61][61];
	void clear()
	{
		for (int x{}; x < 61; ++x)
			for (int y{}; y < 61; ++y)
			{
				arr[x][y].x = 0;
				arr[x][y].y = 0;
			}
	}

	~Path() {};
};

class Ice : public Actor
{
public:
	Ice(int start_X, int start_Y)
		:Actor(IID_ICE, start_X, start_Y, nullptr, none, .25, 3)
	{
		setVisible(true);
	};
	void doSomething() {};


	~Ice() { };
private:
};

class IceMan : public Actor
{
public:
	IceMan(StudentWorld* wrld)
		: Actor(IID_PLAYER, 30, 60, wrld, right, 1.0, 0U),
		water(5),
		sonar(1),
		nuggets(0)
		{
		setVisible(true);
		setHP(10);
	};

	int getWater();
	int getSonar();
	int getNuggets();
	
	virtual void doSomething();
	void CheckIceCollision();
	void incNuggets();
	void incSonar();
	void incWater();

	~IceMan() {

	};
private:
	int water;
	int sonar;
	int nuggets;
};

class Item : public Actor
{
public:
	Item(const int ID, int x, int y, StudentWorld* wrld = nullptr, Direction d = right, double size = 1.0, unsigned int depth = 2U)
		: Actor(ID, x, y, wrld, d, size, depth) {};
	virtual void doSomething() = 0;
	~Item() {};
private:
};

class Protester : public Actor
{
public:
	Protester(const int ID, StudentWorld* wrld)
		: Actor(ID, 60, 60, wrld, left, 1.0, 0U),
		steps(std::max(8, rand() % 60)),
		shoutTicks(0),
		perpTicks(0),
		wait(0),
		GivesUp(false),
		searched(false),
		prev()
	{ 
		setVisible(true);
	};

	virtual void doSomething() = 0;
	virtual void setAnnoyed(int a, bool boulder = false) = 0;
	virtual void gotGold() = 0;
	bool isFacingPlayer();
	bool isBlocked(bool play = false, Direction d = none);
	void decNonRestTicks();
	bool isGiveUp();
	void setGivesUp();
	bool Search(vector2D &Goal, vector2D &start);
	int getWait();
	void setWait(int w);
	void decWait();
	int getShout();
	void setShout(int t);
	int getPerp();
	void setPerp();
	bool hasSearched();
	void setSearched(bool search);
	int getSteps();
	void setSteps(int s);
	void setSteps();
	void decSteps();
	Path* getPath();
	void movePerp();
	void changeDirection();
	void takeStep();

	~Protester() {};
private:
	int wait;
	int steps;
	bool GivesUp;
	bool searched;
	int shoutTicks;
	int perpTicks;
	std::queue<vector2D*> Que;
	std::vector<vector2D*> dis;
	Path prev;
};

class RegularProtester : public Protester
{
public:
	RegularProtester(StudentWorld* wrld) : Protester(IID_PROTESTER, wrld) { setHP(5); };
	void doSomething();
	void setAnnoyed(int a, bool boudler = false);
	void gotGold();


	~RegularProtester() {};
};

class HardCoreProtester : public Protester
{
public:
	HardCoreProtester(StudentWorld* wrld) : Protester(IID_HARD_CORE_PROTESTER, wrld) { setHP(20); };
	void doSomething();
	void setAnnoyed(int a, bool boudler = false);
	void gotGold();
	~HardCoreProtester() {};
};

class Oil : public Item
{
public:
	Oil(int x, int y, StudentWorld* wrld) : Item(IID_BARREL, x, y, wrld) {};
	void doSomething();
	~Oil() {};
private:
};



class Nugget : public Item
{
public:
	Nugget(int x, int y, StudentWorld* wrld, bool temp = false) : Item(IID_GOLD, x, y, wrld), temp(temp)
	{
		if (temp == true)
		{
			setTick(0);
		}
	};
	void doSomething();
	~Nugget() {};
private:
	bool temp;
};

class RandomSpawnItem : public Item
{
public:
	RandomSpawnItem(const int ID, int x, int y, StudentWorld* wrld) : Item(ID, x, y, wrld)
	{
		setTick(0);
		setVisible(true);
	};
	void doSomething();
	~RandomSpawnItem() {};
};

class Sonar : public RandomSpawnItem
{
public:
	Sonar(int x, int y, StudentWorld* wrld) : RandomSpawnItem(IID_SONAR, x, y, wrld) {};
	~Sonar() {};
};

class Water : public RandomSpawnItem
{
public:
	Water(int x, int y, StudentWorld* wrld) : RandomSpawnItem(IID_WATER_POOL, x, y, wrld) {};
	~Water() {};
};


class Squirt : public Actor
{
public:
	Squirt(int x, int y, Direction d, StudentWorld* wrld) : Actor(IID_WATER_SPURT, x, y, wrld, d, 1.0, 1U),
		start_x(x), start_y(y)
	{
		setVisible(true);
	}
	void doSomething();
	~Squirt() {};
private:
	int start_x, start_y;

};

class Boulder : public Actor
{
public:
	Boulder(int x, int y, StudentWorld* wrld) : Actor(IID_BOULDER, x, y, wrld, down, 1.0, 1U), state(stable) { setVisible(true); };

	void doSomething();

	~Boulder() {};
private:
	enum State {stable, waiting, falling} state;
};

#endif // ACTOR_H_
