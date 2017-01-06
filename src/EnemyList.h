#ifndef ENEMYLIST_H
#define ENEMYLIST_H

#include <vector>
#include "GameResourceManager.h"
#include "GameEntityManager.h"
#include "GameEntity.h"
#include <SFML\Graphics\RenderWindow.hpp>

class EnemyList {
public:
	EnemyList(void);
	~EnemyList(void);

	static void create(sf::RenderWindow &win, GameEntityManager &eMgr, GameResourceManager &rMgr, int amount);
	static GameEntity* getEnemy1();
	static GameEntity* getEnemy2();
	static GameEntity* getEnemy3();
	static void clear() { 
		enemy1List.clear(); 
		enemy2List.clear(); 
		enemy3List.clear(); 
	}

	static std::vector<GameEntity*> enemy1List;
	static std::vector<GameEntity*> enemy2List;
	static std::vector<GameEntity*> enemy3List;

private:
	static const float GAME_Z_DISTANCE;
};

#endif
