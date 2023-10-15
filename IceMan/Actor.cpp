#include "Actor.h"
#include "StudentWorld.h"
#include <queue>
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


//////////////////////////////////////
//       ACTOR
//////////////////////////////////////
StudentWorld* Actor::getWorld()
{
	return world;
}

int Actor::getTick()
{
	return tickToReach;
}

void Actor::setTick(int tick)
{
	if (tick == 0)
	{
		tickToReach = 0;
	}
	else tickToReach = tick + world->getCurrentTick();
}

bool Actor::inRadius(int r, Actor* act)
{
	return inRadius(r, act, getX(), getY());
}

bool Actor::inRadius(int r, Actor* act, int x, int y)
{
	//radius of a circle is (x-h)^2 + (y-k)^2 = r^2

	if (std::sqrt(std::pow((act->getX()) - x, 2) + std::pow((act->getY()) - y, 2)) <= r)
	{
		return true;
	}
	else return false;
}

bool Actor::moreThanRadius(int r, Actor* act)
{
	return moreThanRadius(r, act, getX(), getY());
}

bool Actor::moreThanRadius(int r, Actor* act, int x, int y)
{
	//radius of a circle is (x-h)^2 + (y-k)^2 = r^2

	if (std::sqrt(std::pow((act->getX()) - x, 2) + std::pow((act->getY()) - y, 2)) > r)
	{
		return true;
	}
	else return false;
}


int Actor::getHP()
{
	return hp;
}

void Actor::setHP(int ammount)
{
	hp = ammount;
}



//////////////////////////////////////
//       ICEMAN
//////////////////////////////////////


int IceMan::getWater()
{
	return water;
}
int IceMan::getNuggets()
{
	return nuggets;
}
int IceMan::getSonar()
{
	return sonar;
}
void IceMan::doSomething()
{
	if (isAlive() == false)
		return;
	int key;
	if(getWorld()->getKey(key) == true)
	{
		auto actors = getWorld()->GetActors();

		switch (key)
		{

		case KEY_PRESS_LEFT:
			if (getDirection() != left)
			{
				setDirection(left);
				break;
			}
			for (auto it = actors->begin(); it != actors->end(); ++it)
			{
				if ((*it)->getID() == IID_BOULDER)
					if (inRadius(4, (*it)) && (getY() <= (*it)->getY() + 3 && getY() >= (*it)->getY() - 3 && getX() >= (*it)->getX() + 3))
						return;
			}
			if (getX() == 0)
				moveTo(getX(), getY());
			else
				moveTo(getX() - 1, getY());
			break;


		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
			{
				setDirection(right);
				break;
			}
			for (auto it = actors->begin(); it != actors->end(); ++it)
			{
				if ((*it)->getID() == IID_BOULDER)
					if (inRadius(4, (*it)) && (getY() <= (*it)->getY() + 3 && getY() >= (*it)->getY() - 3 && getX() <= (*it)->getX() - 3))
						return;
			}
			if (getX() == 56)
				moveTo(getX(), getY());
			else
				moveTo(1 + getX(), getY());
			break;


		case KEY_PRESS_DOWN:
			if (getDirection() != down)
			{
				setDirection(down);
				break;
			}
			for (auto it = actors->begin(); it != actors->end(); ++it)
			{
				if ((*it)->getID() == IID_BOULDER)
					if (inRadius(4,(*it)) && (getX() <= (*it)->getX() + 3 && getX() >= (*it)->getX() - 3 && getY() >= (*it)->getY() + 3))
						return;
			}
			if (getY() == 0)
				moveTo(getX(), getY());
			else
				moveTo(getX(), getY() - 1);
			break;


		case KEY_PRESS_UP:
			if (getDirection() != up)
			{
				setDirection(up);
				break;
			}
			for (auto it = actors->begin(); it != actors->end(); ++it)
			{
				if ((*it)->getID() == IID_BOULDER)
					if (inRadius(4, (*it)) && (getX() <= (*it)->getX() + 3 && getX() >= (*it)->getX() - 3 && getY() <= (*it)->getY() - 3))
						return;
			}
			if (getY() == 60)
				moveTo(getX(), getY());
			else
				moveTo(getX(), 1 + getY());
			break;


		case KEY_PRESS_ESCAPE:
			setHP(0);
			setAlive(false);
			break;


		case KEY_PRESS_TAB:
			if (nuggets > 0)
			{
				actors->push_back(new Nugget(getX(), getY(), getWorld(), true));
				--nuggets;
			}
			break;


		case KEY_PRESS_SPACE:
			if (water > 0)
			{
				switch (getDirection())
				{
				case up:
					actors->push_back(new Squirt(getX(), getY() + 4, getDirection(), getWorld()));
					break;
				case down:
					actors->push_back(new Squirt(getX(), getY() - 4, getDirection(), getWorld()));
					break;
				case right:
					actors->push_back(new Squirt(getX() + 4, getY(), getDirection(), getWorld()));
					break;
				case left:
					actors->push_back(new Squirt(getX() - 4, getY(), getDirection(), getWorld()));
					break;
				}
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				--water;
			}
			break;
		case 'z':
		case 'Z':
			if (sonar > 0)
			{
				for (auto it = actors->begin(); it != actors->end(); ++it)
				{
					if (inRadius(12, *it) == true)
					{
						(*it)->setVisible(true);
					}
				}
				--sonar;
				getWorld()->playSound(SOUND_SONAR);
			}
			break;
		}
		CheckIceCollision();
		if (getHP() <= 0 && isAlive() == true)
		{
			setAlive(false);
			getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		}
	}
}

void IceMan::CheckIceCollision()
{
	auto ice = getWorld()->getIce();
	for(int x = getX(); x < getX() + 4; ++x)
		for (int y = getY(); y < getY() + 4; ++y)
		{
			if (x > 59 || y > 59 || x < 0 || y < 0)
				continue;
			if (ice->arr[x][y] != nullptr) {
				delete ice->arr[x][y];
				ice->arr[x][y] = nullptr;
				getWorld()->playSound(SOUND_DIG);
			}
		}
}

void IceMan::incNuggets()
{
	++nuggets;
}
//bool IceMan::decNuggets()
//{
//	if (nuggets <= 0) return false;
//	--nuggets;
//	return true;
//}

void IceMan::incSonar()
{
	++sonar;
}

void IceMan::incWater()
{
	water += 5;
}

//////////////////////////////////////
//       PROTESTER
//////////////////////////////////////
bool Protester::isGiveUp()
{
	return GivesUp;
}
void Protester::setGivesUp()
{
	GivesUp = true;
}
int Protester::getWait()
{
	return wait;
}
void Protester::setWait(int w)
{
	wait = w;
}
void Protester::decWait()
{
	--wait;
}
int Protester::getShout()
{
	return shoutTicks;
}
void Protester::setShout(int t)
{
	shoutTicks = t;
}
int Protester::getPerp()
{
	return perpTicks;
}
void Protester::setPerp()
{
	perpTicks = 200;
}
bool Protester::hasSearched()
{
	return searched;
}
void Protester::setSearched(bool search)
{
	searched = search;
}
int Protester::getSteps()
{
	return steps;
}
void Protester::setSteps(int s)
{
	steps = s;
}
void Protester::setSteps()
{
	steps = std::max(8, rand() % 61);
}
void Protester::decSteps()
{
	--steps;
}
Path* Protester::getPath()
{
	return &prev;
}
void Protester::decNonRestTicks()
{
	--shoutTicks;
	--perpTicks;
}

bool Protester::isFacingPlayer()
{
	IceMan* player = getWorld()->getPlayer();
	switch (getDirection())
	{
	case up:
		if (player->getX() == getX() && player->getY() >= getY())
			return true;
		break;
	case down:
		if (player->getX() == getX() && player->getY() <= getY())
			return true;
		break;
	case left:
		if (player->getX() <= getX() && player->getY() == getY())
			return true;
		break;
	case right:
		if (player->getX() >= getX() && player->getY() == getY())
			return true;
		break;
	}
	return false;
}

bool Protester::isBlocked(bool play, Direction d)
{
	IceArray* ice = getWorld()->getIce();
	IceMan* player = getWorld()->getPlayer();
	auto actors = getWorld()->GetActors();
	int maxX, maxY;
	if (play)
	{
		maxX = player->getX();
		maxY = player->getY();
		if (player->getX() >= getX() && player->getY() == getY()) d = right;
		if (player->getX() <= getX() && player->getY() == getY()) d = left;
		if (player->getX() == getX() && player->getY() <= getY()) d = down;
		if (player->getX() == getX() && player->getY() >= getY()) d = up;
	}
	else {
		if(d == none)
			d = getDirection();
		switch (d)
		{
		case up:
			maxY = getY() + 5;
			break;
		case down:
			maxY = getY() - 1;
			break;
		case left:
			maxX = getX() - 1;
			break;
		case right:
			maxX = getX() + 5;
		}
	}
	switch (d)
	{
	case up:
		for (int x = getX(); x < getX() + 4; ++x)
			for (int y = getY(); y < maxY; ++y)
			{
				if (x >= 60 || x < 0 || y >= 60 || y < 0)
					continue;
				if (ice->arr[x][y] != nullptr)
					return true;
			}
		for (auto it = actors->begin(); it != actors->begin(); ++it)
			if ((*it)->getID() == IID_BOULDER)
				for (int y = getY(); y < maxY; ++y)
					if (inRadius(3, (*it), getX(), y))
						return true;
		break;
	case down:
		for (int x = getX(); x < getX() + 4; ++x)
			for (int y = getY(); y >= maxY; --y)
			{
				if (x >= 60 || x < 0 || y >= 60 || y < 0)
					continue;
				if (ice->arr[x][y] != nullptr)
					return true;
			}
		for (auto it = actors->begin(); it != actors->begin(); ++it)
			if ((*it)->getID() == IID_BOULDER)
				for (int y = getY(); y >= maxY; ++y)
					if (inRadius(3, (*it), getX(), y))
						return true;
		break;
	case left:
		for (int y = getY(); y < getY() + 4; ++y)
			for (int x = getX(); x >= maxX; --x)
			{
				if (x >= 60 || x < 0 || y >= 60 || y < 0)
					continue;
				if (ice->arr[x][y] != nullptr)
					return true;
			}
		for (auto it = actors->begin(); it != actors->begin(); ++it)
			if ((*it)->getID() == IID_BOULDER)
				for (int x = getX(); x >= maxX; ++x)
					if (inRadius(3, (*it), x, getY()))
						return true;
		break;
	case right:
		for (int y = getY(); y < getY() + 4; ++y)
			for (int x = getX() + 4; x < maxX; ++x)
			{
				if (x >= 60 || x < 0 || y >= 60 || y < 0)
					continue;
				if (ice->arr[x][y] != nullptr)
					return true;
			}
		for (auto it = actors->begin(); it != actors->begin(); ++it)
			if ((*it)->getID() == IID_BOULDER)
				for (int x = getX() + 4; x < maxX; ++x)
					for(int y = getY(); y < getY() + 4; ++y)
					if (inRadius(4, (*it), x, y))
						return true;
		break;
	}
	return false;
}


bool Protester::Search(vector2D &G, vector2D &start)
{
	IceArray* ice = getWorld()->getIce();
	auto actors = getWorld()->GetActors();
	while (!Que.empty())
	{
		Que.pop();
	}
	dis.clear();
	prev.clear();
	dis.push_back(&start);
	Que.push(&start);
	while (!Que.empty())
	{
		auto v = Que.front();
		Que.pop();
		if (G.x == v->x && G.y == v->y)
		{
			return true;
		}

		vector2D* up = new vector2D(v->x, v->y + 1), *down = new vector2D(v->x, v->y - 1), *left = new vector2D(v->x - 1, v->y), *right = new vector2D(v->x + 1, v->y);
		vector2D* edge[4] = { up, down, left, right };
		for (auto w : edge)
		{
			if (w->x < 0 || w->x > 60 || w->y < 0 || w->y > 60)
				continue;
			bool discov = false;
			for (auto d : dis)
			{
				if (w->x == d->x && w->y == d->y)
				{
					discov = true;
					break;
				}
			}
			if (!discov)
			{
				dis.push_back(w);
				bool isIce{ false };
				for (int x = w->x; x < w->x + 4; ++x)
				{
					for (int y = w->y; y < w->y + 4; ++y)
					{
						if (x >= 60 || y >= 60) continue;
						if (ice->arr[x][y] != nullptr)
						{
							isIce = true;
							break;
						}
					}
					if (isIce) break;
				}
				for (auto it = actors->begin(); it != actors->end(); ++it)
				{
					if ((*it)->getID() == IID_BOULDER)
					{
						if (inRadius(4, (*it), w->x, w->y))
						{
							isIce = true;
							break;
						}
					}
				}
				if (!isIce)
				{
					Que.push(w);
					prev.arr[w->x][w->y] = *v;
				}
			}
		}
	}
	return false;
}

void Protester::movePerp()
{
	if (getX() == 60 && getY() == 60) return;
	auto actors = getWorld()->GetActors();
	IceArray* ice = getWorld()->getIce();
	bool intRight{ true }, intLeft{ true }, intUp{ true }, intDown{ true };
	switch (getDirection())
	{
	case up:
	case down:
		intUp = false;
		intDown = false;
		for (int y = getY(); y < getY() + 4; ++y)
		{
			int x{};
			if (intRight)
			{
				x = getX() + 4;
				if (ice->arr[x][y] != nullptr)
					intRight = false;
			}
			if (!intLeft && !intRight) break;
			if (intLeft)
			{
				x = getX() - 1;
				if (ice->arr[x][y] != nullptr)
					intLeft = false;
			}
			if (!intLeft && !intRight) break;
		}
		if (!(intLeft && intRight)) break;
		for (auto it = actors->begin(); it != actors->end(); ++it)
		{
			if ((*it)->getID() == IID_BOULDER)
			{
				if ((*it)->getY() - 3 >= getY() && (*it)->getY() + 3 <= getY())
				{
					if ((*it)->getX() == getX() + 4)
					{
						intRight = false;
					}
					if (!intLeft && !intRight) break;
					if ((*it)->getX() == getX() - 1)
					{
						intLeft = false;
					}
				}
			}
			if (!intLeft && !intRight) break;
		}
		if (intLeft && intRight)
		{
			int r = rand() % 2;
			if (r == 0) setDirection(right);
			if (r == 1) setDirection(left);
			setPerp();
			setSteps();
			break;
		}
		if (intLeft) setDirection(left);
		if (intRight) setDirection(right);
		if (intRight || intLeft)
		{
			setPerp();
			setSteps();
		}
		break;
	case left:
	case right:
		intLeft = false;
		intRight = false;
		for (int x = getX(); x < getX() + 4; ++x)
		{
			int y{};
			if (intUp)
			{
				y = getY() + 4;
				if (ice->arr[x][y] != nullptr)
					intUp = false;
			}
			if (!intUp && !intDown) break;
			if (intDown)
			{
				y = getY() - 1;
				if (ice->arr[x][y] != nullptr)
					intDown = false;
			}
			if (!intUp && !intDown) break;
		}
		if (!intUp && !intDown) break;
		for (auto it = actors->begin(); it != actors->end(); ++it)
		{
			if ((*it)->getID() == IID_BOULDER)
			{
				if ((*it)->getX() - 3 >= getX() && (*it)->getX() + 3 <= getX())
				{

					if ((*it)->getY() == getY() + 4)
					{
						intUp = false;
					}
					if (!intUp && !intDown) break;
					if ((*it)->getY() == getY() - 1)
					{
						intDown = false;
					}
				}
			}
			if (!intUp && !intDown) break;
		}
		if (intUp && intDown)
		{
			int r = rand() % 2;
			if (r == 0) setDirection(up);
			if (r == 1) setDirection(down);
			setPerp();
			setSteps();
			break;
		}
		if (intDown) setDirection(down);
		if (intUp) setDirection(up);
		if (intUp || intDown)
		{
			setPerp();
			setSteps();
		}
		break;
	}
}

void Protester::changeDirection()
{
	bool canGo = false;
	while (canGo == false)
	{
		int d{ rand() % 4 };
		switch (d)
		{
		case 0:
			if (getY() < 60 && !isBlocked(false, up))
			{
				canGo = true;
				setDirection(up);
			}
			break;
		case 1:
			if (getY() > 0 && !isBlocked(false, down))
			{
				canGo = true;
				setDirection(down);
			}
			break;
		case 2:
			if (getX() > 0 && !isBlocked(false, left))
			{
				canGo = true;
				setDirection(left);
			}
			break;
		case 3:
			if (getX() < 56 && !isBlocked(false, right))
			{
				canGo = true;
				setDirection(right);
			}
			break;
		}
	}
	setSteps();
}

void Protester::takeStep()
{
	if (!isBlocked())
	{
		switch (getDirection())
		{
		case up:
			if (getY() < 60)
				moveTo(getX(), getY() + 1);
			else setSteps(0);
			break;
		case down:
			if (getY() > 0)
				moveTo(getX(), getY() - 1);
			else setSteps(0);
			break;
		case left:
			if (getX() > 0)
				moveTo(getX() - 1, getY());
			else setSteps(0);
			break;
		case right:
			if (getX() < 56)
				moveTo(getX() + 1, getY());
			else setSteps(0);
			break;
		}
		decNonRestTicks();
	}
	else setSteps(0);
}



//////////////////////////////////////
//       REGULAR PROTESTER
//////////////////////////////////////


void RegularProtester::gotGold()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	getWorld()->increaseScore(25);
	setGivesUp();
}

void RegularProtester::setAnnoyed(int a, bool boulder)
{
	setHP(getHP() - a);
	if (getHP() <= 0)
	{
		setGivesUp();
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setWait(0);
		if (boulder) getWorld()->increaseScore(500);
		else getWorld()->increaseScore(100);
	}
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	setWait(std::max(50, 100 - (int)getWorld()->getLevel() * 10));
}

void RegularProtester::doSomething()
{
	IceMan* player = getWorld()->getPlayer();
	auto actors = getWorld()->GetActors();
	IceArray* ice = getWorld()->getIce();
	if (isAlive() == false) return;

	if (getWait() > 0)
	{
		decWait();
		return;
	}
	setWait(std::max(0, 3 - (int)getWorld()->getLevel() / 4));
	if (isGiveUp() && !hasSearched())
	{
		setSearched(Search(vector2D(getX(), getY()), vector2D(60, 60)));
		return;
	}
	if (isGiveUp())
	{
		if (getX() == 60 && getY() == 60)
		{
			setAlive(false);
			return;
		}
		int x, y;
		auto v = getPath()->arr[x = getX()][y = getY()];
		if (v.x > x)
			setDirection(right);
		if (v.x < x)
			setDirection(left);
		if (v.y > y)
			setDirection(up);
		if (v.y < y)
			setDirection(down);
		moveTo(v.x, v.y);
		return;
	}
	if (inRadius(4, player) && isFacingPlayer() && getShout() <= 0)
	{
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		player->setHP(player->getHP() - 2);
		setShout(15);
		return;
	}
	if ((getX() == player->getX() || getY() == player->getY()) && !isBlocked(true) && moreThanRadius(4, player))
	{
		Direction d;
		if (player->getX() > getX() && player->getY() == getY()) d = right;
		else if (player->getX() < getX() && player->getY() == getY()) d = left;
		else if (player->getX() == getX() && player->getY() < getY()) d = down;
		else if (player->getX() == getX() && player->getY() > getY()) d = up;
		switch (d)
		{
		case up:
			setDirection(up);
			moveTo(getX(), getY() + 1);
			break;
		case down:
			setDirection(down);
			moveTo(getX(), getY() - 1);
			break;
		case left:
			setDirection(left);
			moveTo(getX() - 1, getY());
			break;
		case right:
			setDirection(right);
			moveTo(getX() + 1, getY());
			break;
		case none:
			break;
		}
		setSteps(0);
		decNonRestTicks();
		return;
	}
	decSteps();
	if (getSteps() <= 0)
	{
		changeDirection();
	}
	if (getPerp() <= 0)
	{
		movePerp();
	}
	takeStep();
}

//////////////////////////////////////
//       HARDCORE PROTESTER
//////////////////////////////////////


void HardCoreProtester::gotGold()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
	getWorld()->increaseScore(50);
	setWait(std::max(50, 100 - (int)getWorld()->getLevel() * 10));
}

void HardCoreProtester::setAnnoyed(int a, bool boulder)
{
	setHP(getHP() - a);
	if (getHP() <= 0)
	{
		setGivesUp();
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setWait(0);
		if (boulder) getWorld()->increaseScore(500);
		else getWorld()->increaseScore(250);
	}
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	setWait(std::max(50, 100 - (int)getWorld()->getLevel() * 10));
}

void HardCoreProtester::doSomething()
{
	IceMan* player = getWorld()->getPlayer();
	auto actors = getWorld()->GetActors();
	IceArray* ice = getWorld()->getIce();


	if (!isAlive()) return;
	if (getWait() > 0)
	{
		decWait();
		return;
	}
	setWait(std::max(0, 3 - (int)getWorld()->getLevel() / 4));
	if (isGiveUp() && !hasSearched())
	{
		setSearched(Search(vector2D(getX(), getY()), vector2D(60, 60)));
		return;
	}
	if (isGiveUp())
	{
		if (getX() == 60 && getY() == 60)
		{
			setAlive(false);
			return;
		}
		int x, y;
		auto v = getPath()->arr[x = getX()][y = getY()];
		if (v.x > x)
			setDirection(right);
		if (v.x < x)
			setDirection(left);
		if (v.y > y)
			setDirection(up);
		if (v.y < y)
			setDirection(down);
		moveTo(v.x, v.y);
		return;
	}
	if (inRadius(4, player) && isFacingPlayer() && getShout() <= 0)
	{
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		player->setHP(player->getHP() - 2);
		setShout(15);
		return;
	}
	if (moreThanRadius(4, player))
	{
		Search(vector2D(getX(), getY()), vector2D(player->getX(), player->getY()));
		int M = 16 + getWorld()->getLevel() * 2;
		bool isLessThanM{ false };
		vector2D v{ getPath()->arr[getX()][getY()] };
		//std::cout << v.x << "," << v.y << std::endl;
		for (int i{}; i < M; ++i)
		{
			//std::cout << v.x << "," << v.y << std::endl;
			if (v.x == player->getX() && v.y == player->getY())
				isLessThanM = true;
			v = getPath()->arr[v.x][v.y];
		}
		v = getPath()->arr[getX()][getY()];
		if (isLessThanM)
		{
			if (getX() < v.x)
				setDirection(right);
			if (getX() > v.x)
				setDirection(left);
			if (getY() < v.y)
				setDirection(up);
			if (getY() > v.y)
				setDirection(down);
			moveTo(v.x, v.y);
			return;
		}
	}
	if (moreThanRadius(4, player) && !isBlocked(true) && (getX() == player->getX() || getY() == player->getY()))
	{
		Direction d;
		if (player->getX() > getX() && player->getY() == getY()) d = right;
		else if (player->getX() < getX() && player->getY() == getY()) d = left;
		else if (player->getX() == getX() && player->getY() < getY()) d = down;
		else if (player->getX() == getX() && player->getY() > getY()) d = up;
		switch (d)
		{
		case up:
			setDirection(up);
			moveTo(getX(), getY() + 1);
			break;
		case down:
			setDirection(down);
			moveTo(getX(), getY() - 1);
			break;
		case left:
			setDirection(left);
			moveTo(getX() - 1, getY());
			break;
		case right:
			setDirection(right);
			moveTo(getX() + 1, getY());
			break;
		case none:
			break;
		}
		setSteps(0);
		decNonRestTicks();
		return;
	}
	decSteps();
	if (getSteps() <= 0)
	{
		changeDirection();
	}
	if(getPerp() <= 0)
	{
		movePerp();
	}
	takeStep();
}






//////////////////////////////////////
//       BOULDER
//////////////////////////////////////

void Boulder::doSomething()
{
	if (isAlive() == false)
		return;
	IceArray* ice = getWorld()->getIce();
	int y = getY();
	bool theresIce = false;
	switch (state)
	{
	case stable:
		
		for (int x = getX(); x < getX() + 4; ++x)
		{
			if (ice->arr[x][y - 1] != nullptr)
				theresIce = true;
		}
		if (theresIce == false)
		{
			state = waiting;
			setTick(30);
		}
		break;
	case waiting:
		if (getTick() <= getWorld()->getCurrentTick())
		{
			state = falling;
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		break;
	case falling:
		std::vector<Actor*>* actors = getWorld()->GetActors();
		bool theresObject = false;
		for (int x = getX(); x < getX() + 4; ++x)
		{
			if (ice->arr[x][y - 1] != nullptr)
				theresObject = true;
		}
		for (auto it = actors->begin(); it != actors->end(); ++it)
		{
			
			if ((*it)->getY() == y - 4 && ((*it)->getX() <= getX() - 3 || (*it)->getX() >= getX() + 3))
			{
				if ((*it)->getID() == IID_BOULDER)
					theresObject = true;
			}
			if (inRadius(3, *it) == true)
			{
				switch ((*it)->getID())
				{
				case IID_PROTESTER:
				case IID_HARD_CORE_PROTESTER:
					if ((*it)->isGiveUp()) continue;
					(*it)->setAnnoyed(100, true);
					break;
				}
			}
		}
		if (inRadius(3, getWorld()->getPlayer()))
		{
			getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
			getWorld()->getPlayer()->setAlive(false);
			setAlive(false);
		return;
		}


		if (y < 0 || theresObject == true)
		{
			setAlive(false);
		}
		else
		{
			moveTo(getX(), getY() - 1);
		}
	}
}

//////////////////////////////////////
//       Oil
//////////////////////////////////////

void Oil::doSomething()
{
	if (isAlive() == false)
		return;
	StudentWorld* world = getWorld();
	IceMan* player = world->getPlayer();
	if (isVisible() == false && inRadius(4, player))
	{
		setVisible(true);
		return;
	}
	else if (inRadius(3, player))
	{
		setAlive(false);
		setVisible(false);
		world->playSound(SOUND_FOUND_OIL);
		world->increaseScore(1000);
		world->decOil();
	}
}

//////////////////////////////////////
//       Nuggets
//////////////////////////////////////

void Nugget::doSomething()
{
	if (isAlive() == false)
		return;
	StudentWorld* world = getWorld();
	IceMan* player = world->getPlayer();
	if (isVisible() == false && inRadius(4, player))
	{
		setVisible(true);
		return;
	}
	else if (inRadius(3, player) && temp == false)
	{
		setAlive(false);
		setVisible(false);
		world->playSound(SOUND_GOT_GOODIE);
		world->increaseScore(10);
		player->incNuggets();
	}
	else if (temp == true) 
	{
		if (getTick() == 0)
		{
			setTick(100);
		}
		if (getTick() < getWorld()->getCurrentTick())
		{
			setAlive(false);
			return;
		}

		auto actors = getWorld()->GetActors();
		for (auto it = actors->begin(); it != actors->end(); ++it)
		{
			if ((*it)->getID() == IID_PROTESTER
				|| (*it)->getID() == IID_HARD_CORE_PROTESTER)
			{
				if ((*it)->isGiveUp()) continue;
				else if (inRadius(3, (*it)))
				{
					setAlive(false);
					(*it)->gotGold();
					break;
				}
			}
		}
	}
}

//////////////////////////////////////
//       Squirt
//////////////////////////////////////

void Squirt::doSomething()
{
	if (isAlive() == false)
	{
		return;
	}
	int dest_x = getX(), dest_y = getY();// , ice_x{}, ice_y{};
	bool destIsBlocked = false;
	switch (getDirection())
	{
	case up:
		++dest_y;
		//ice_y = dest_y + 3;
		break;
	case down:
		--dest_y;
		//ice_y = dest_y;
		break;
	case right:
		++dest_x;
		//ice_x = dest_x + 3;
		break;
	case left:
		--dest_x;
		//ice_x = dest_x;
		break;
	}

	auto actors = getWorld()->GetActors();
	
	for (auto it = actors->begin(); it != actors->end(); ++it)
	{
		
		if (inRadius(3, *it))
		{
			switch ((*it)->getID())
			{
			case IID_PROTESTER:
			case IID_HARD_CORE_PROTESTER:
				if ((*it)->isGiveUp()) continue;
				else
				{
					(*it)->setAnnoyed(2);
					setAlive(false);
				}
				break;
			}
		}
		//checks the boulders but doesnt do anything but set a bool to true
		//if there is a boulder in the destination to prevent bugs
		//such as there being a protester infront of the boulder and it
		//never checked it as you will always check the boulders first and the
		//protesters last
		if ((*it)->getID() == IID_BOULDER)
		{
			if (inRadius(3, (*it), dest_x, dest_y) == true)
			{
				destIsBlocked = true;
			}
		}
	}
	if (dest_x >= start_x + 4 || dest_x <= start_x - 4 
		|| dest_y >= start_y + 4 || dest_y <= start_y - 4)
	{
		setAlive(false);
		return;
	}

	//if there is a boulder
	if (destIsBlocked == true)
	{
		setAlive(false);
		return;
	}
	IceArray *ice = getWorld()->getIce();
	switch (getDirection())
	{
	case up:
	case down:
		for (int x = dest_x; x < dest_x + 3; ++x)
		{
			for (int y = getY(); y < getY() + 4; ++y)
			{
				if (x < 0 || x >= 60 || y >= 60 || y < 0) continue;
				if (ice->arr[x][y] != nullptr)
				{
					setAlive(false);
					return;
				}
			}
		}
		break;
	case left:
	case right:
		for (int y = dest_y; y < dest_y + 3; ++y)
		{
			for (int x = getY(); x < getX() + 4; ++x)
			{
				if (y < 0 || y >= 60 || x >= 60 || x < 0) continue;
				if (ice->arr[x][y] != nullptr)
				{
					setAlive(false);
					return;
				}
			}
		}
		break;
	}

	if (isAlive() == true)
	{
		moveTo(dest_x, dest_y);
	}
}


//////////////////////////////////////
//       RandomSpawnItem
//////////////////////////////////////

void RandomSpawnItem::doSomething()
{
	if (getTick() == 0)
	{
		setTick(std::max(100, 300 - 10 * (int)getWorld()->getLevel()));
	}
	if (isAlive() == false)
		return;

	IceMan* player = getWorld()->getPlayer();
	if (inRadius(3, player) == true)
	{
		setAlive(false);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		switch (getID())
		{
		case IID_SONAR:
			player->incSonar();
			getWorld()->increaseScore(75);
			break;
		case IID_WATER_POOL:
			player->incWater();
			getWorld()->increaseScore(100);
			break;
		}

	}

	if (getTick() < getWorld()->getCurrentTick())
	{
		setAlive(false);
	}
}