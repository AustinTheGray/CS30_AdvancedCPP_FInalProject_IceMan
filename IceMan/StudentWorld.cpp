#include "StudentWorld.h"
#include <string>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

void StudentWorld::UpdateDisplayText()
{
	int lvl = getLevel(),
		lives = getLives(),
		h = player->getHP(),
		w = player->getWater(),
		g = player->getNuggets(),
		o = RemainingOil,
		snr = player->getSonar(),
		scr = getScore();

	std::string text;
	text = "Lvl: "; if (lvl < 10) text += " "; text += std::to_string(lvl);
	text += " Lives: " + std::to_string(lives);
	text += " Hlth: "; if (h < 10) text += " "; text += std::to_string(10 * h) + "%";
	text += " Wtr: "; if (w < 10) text += " "; text += std::to_string(w);

	text += " Gld: "; if (g < 10) text += " "; text += std::to_string(g);
	text += " Oil Left: "; if (o < 10) text += " "; text += std::to_string(o);
	text += " Sonar: "; if (snr < 10) text += " "; text += std::to_string(snr);
	text += " Scr: ";
	if (scr < 100000)
		text += "0";
	if (scr < 10000)
		text += "0";
	if (scr < 1000)
		text += "0";
	if (scr < 100)
		text += "0";
	if (scr < 10)
		text += "0";
	text += std::to_string(scr);

	setGameStatText(text);
}

void StudentWorld::setUpIce()
{
	for (int x{}; x < 60; ++x)
		for (int y{}; y < 60; ++y)
		{
			if (x >= 30 && x <= 33 && y > 4)
			{
				ice.arr[x][y] = nullptr;
				continue;
			}
			ice.arr[x][y] = new Ice(x, y);
		}
}

void StudentWorld::deleteIce()
{
	for (int x{}; x < 60; ++x)
		for (int y{}; y < 60; ++y)
		{
			if (ice.arr[x][y] == nullptr)
				continue;
			delete ice.arr[x][y];
		}
}

bool StudentWorld::inRadius(int r, Actor* act, int x, int y)
{
	//radius of a circle is (x-h)^2 + (y-k)^2 = r^2


	if ((std::pow((act->getX() + SPRITE_WIDTH) - x, 2) + std::pow((act->getY() + SPRITE_HEIGHT) - y, 2)) <= std::pow(r, 2))   //from item's 0,0
	{
		return true;
	}
	else if ((std::pow(act->getX() - (x + SPRITE_WIDTH), 2) + std::pow((act->getY() + SPRITE_HEIGHT) - y, 2)) <= std::pow(r, 2))   //from item's 1,0
	{
		return true;
	}
	else if ((std::pow(act->getX() - (x + SPRITE_WIDTH), 2) + std::pow(act->getY() - (y + SPRITE_HEIGHT), 2)) <= std::pow(r, 2))   //from item's 1,1
	{
		return true;
	}
	else if ((std::pow((act->getX() + SPRITE_WIDTH) - x, 2) + std::pow(act->getY() - (y + SPRITE_HEIGHT), 2)) <= std::pow(r, 2))   //from item's 0,1
	{
		return true;
	}
	else return false;
}

void StudentWorld::setUpActors()
{
	int B = std::min((int)getLevel() / 2 + 2, 9);
	int G = std::max(5 - (int)getLevel() / 2, 2);
	int L = std::min(2 + (int)getLevel(), 21);
	int x, y;
	
	for (B; B > 0; --B)
	{
		bool canPlace = false;
		while (canPlace == false)
		{
			canPlace = true;
			x = rand() % 60;
			y = rand() % 56;
			if (y < 20)
			{
				canPlace = false;
				continue;
			}
			//checks if there are ice blocks in location.
			//if not go back to beginning of while loop and get new position
			for (int i = x; i < x + 4; ++i)
			{
				for (int j = y; j < y + 4; ++j)
				{
					if (canPlace == false) break;
					if (i > 59 || j > 59)
						canPlace = false;
					else if (ice.arr[i][j] == nullptr)
						canPlace = false;
				}
				if (canPlace == false) break;
			}
			//checks if other objects are too close.
			//if yes go back to beginning of while loop and get new position
			for (auto it = actors.begin(); it != actors.end(); ++it)
			{
				if (inRadius(6, (*it), x, y))
				{
					canPlace = false;
					break;
				}
			}
		}
		actors.push_back(new Boulder(x, y, this));
		//removes ice blocks in Boulders Position.
		for (int i = x; i < x + 4; ++i)
			for (int j = y; j < y + 4; ++j)
			{
				if (ice.arr[i][j] != nullptr)
				{
					delete ice.arr[i][j];
					ice.arr[i][j] = nullptr;
				}
			}
	}

	for (G; G > 0; --G)
	{
		bool canPlace = false;
		while (canPlace == false)
		{
			canPlace = true;
			x = rand() % 60;
			y = rand() % 56;
			//checks if there are ice blocks in location.
			//if not go back to beginning of while loop and get new position
			for (int i = x; i < x + 4; ++i)
			{
				for (int j = y; j < y + 4; ++j)
				{
					if (canPlace == false) break;
					if (i > 59 || j > 59)
						canPlace = false;
					else if (ice.arr[i][j] == nullptr)
						canPlace = false;
				}
				if (canPlace == false) break;
			}
			//checks if other objects are too close.
			//if yes go back to beginning of while loop and get new position
			for (auto it = actors.begin(); it != actors.end(); ++it)
			{
				if (inRadius(6, (*it), x, y))
				{
					canPlace = false;
					break;
				}
			}
		}
		actors.push_back(new Nugget(x, y, this));
	}

	RemainingOil = L;
	for (L; L > 0; --L)
	{
		bool canPlace = false;
		while (canPlace == false)
		{
			canPlace = true;
			x = rand() % 60;
			y = rand() % 56;
			//checks if there are ice blocks in location.
			//if not go back to beginning of while loop and get new position
			for (int i = x; i < x + 4; ++i)
			{
				for (int j = y; j < y + 4; ++j)
				{
					if (canPlace == false) break;
					if (i > 59 || j > 59)
						canPlace = false;
					else if (ice.arr[i][j] == nullptr)
						canPlace = false;
				}
				if (canPlace == false) break;
			}
			//checks if other objects are too close.
			//if yes go back to beginning of while loop and get new position
			for (auto it = actors.begin(); it != actors.end(); ++it)
			{
				if (inRadius(6, (*it), x, y))
				{
					canPlace = false;
					break;
				}
			}
		}
		actors.push_back(new Oil(x, y, this));
	}
}

void StudentWorld::deleteActors()
{
	
	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
	actors.clear();
}

void StudentWorld::removeDeadObjects()
{
	auto it = actors.begin();
	while (it != actors.end())
	{
		if ((*it)->isAlive() == false)
		{
			(*it)->setVisible(false);
			delete (*it);
			(*it) = nullptr;
			actors.erase(it);
			it = actors.begin();
		}
		else ++it;
	}
}

void StudentWorld::addNewActors()
{
	int T = std::max(25, 200 - (int)getLevel());
	int P = std::min((double)15, 2 + getLevel() * 1.5);
	int HC = std::min(90, (int)getLevel() * 10 + 30);
	int p{};
	for (auto it : actors)
	{
		if ((*it).getID() == IID_PROTESTER
			|| (*it).getID() == IID_HARD_CORE_PROTESTER)
			++p;
	}
	if (nextProtester <= currentTick)
	{
		nextProtester = T + currentTick;
		if (p < P)
			if (rand() % HC == 1)
				actors.push_back(new HardCoreProtester(this));
			else
				actors.push_back(new RegularProtester(this));
	}

	int G = getLevel() * 25 + 300;
	if (rand() % G == 1)
		if (rand() % 5 == 1) //add sonar
		{
			actors.push_back(new Sonar(0, 60, this));
		}
		else //add water
		{
			bool canPlace{ false };
			int x, y;
			while (canPlace == false)
			{
				canPlace = true;
				x = rand() % 60;
				y = rand() % 56;
				//checks if there are ice blocks in location.
				//if not go back to beginning of while loop and get new position
				for (int i = x; i < x + 4; ++i)
				{
					for (int j = y; j < y + 4; ++j)
					{
						if (canPlace == false) break;
						if (i > 59 || j > 59)
							canPlace = false;
						else if (ice.arr[i][j] != nullptr)
							canPlace = false;
					}
					if (canPlace == false) break;
				}
			}
			actors.push_back(new Water(x, y, this));
		}
}


void StudentWorld::decOil()
{
	--RemainingOil;
}

IceArray* StudentWorld::getIce()
{
	return &ice;
}

IceMan* StudentWorld::getPlayer()
{
	return player;
}