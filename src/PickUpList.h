#ifndef PICKUPLIST_H
#define PICKUPLIST_H

#include <vector>
#include "GameResourceManager.h"
#include "GameEntityManager.h"
#include "GameEntity.h"
#include <SFML\Graphics\RenderWindow.hpp>

class PickUpList {
public:
	PickUpList(void);
	~PickUpList(void);

	static void create(sf::RenderWindow &win, GameEntityManager &eMgr, GameResourceManager &rMgr, int amount);
	static GameEntity* getPickUp();
	static void clear() { pickUpList.clear(); }

	static std::vector<GameEntity*> pickUpList;

private:
	static const float GAME_Z_DISTANCE;
};

#endif