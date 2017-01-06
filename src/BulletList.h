#ifndef BULLETLIST_H
#define BULLETLIST_H

#include <vector>
#include "GameResourceManager.h"
#include "GameEntityManager.h"
#include "GameEntity.h"
#include <SFML\Graphics\RenderWindow.hpp>

class BulletList {
public:
	BulletList(void);
	~BulletList(void);

	static void create(sf::RenderWindow &win, GameEntityManager &eMgr, GameResourceManager &rMgr, int amount);
	static GameEntity* getBullet();
	static void clear() { bulletList.clear(); }

	static std::vector<GameEntity*> bulletList;

private:
	static const float GAME_Z_DISTANCE;
};

#endif

